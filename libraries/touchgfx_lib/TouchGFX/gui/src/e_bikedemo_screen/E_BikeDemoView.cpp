#include <gui/e_bikedemo_screen/E_BikeDemoView.hpp>
#include <gui/common/Utils.hpp>
#include <gui/common/FloatEasingEquations.hpp>

#include <images/BitmapDatabase.hpp>
#include <touchgfx/Utils.hpp>

#include <cmath>
#include <string>

// mph scale
//
//  -PI/2            0.0             PI/2
//    |               |               |
//    +---------------+---------------+
//    |               |               |
//   0.0 km/h       30.0 km/h       60.0 km/h
//
//
// mph scale
//
//  -PI/2            0.0             PI/2
//    |               |               |
//    +---------------+---------------+
//    |               |               |
//   0.0 mph        18.0 mph        36.0 mph

namespace
{
static constexpr auto PI_2 = 1.570f;
static constexpr auto ticks_disable_simulation_period = 1000;
static constexpr auto velocity_max_kmh = 60.0f;
static constexpr auto velocity_max_mph = 36.0f;
static constexpr auto screen_center_x = 400;
static constexpr auto screen_center_y = 240;
static constexpr auto speedometer_center_x = 400.0f;
static constexpr auto speedometer_center_y = 240.0f;
static constexpr auto outer_handle_distance_squared = 125 * 125;

//Compass
static constexpr auto rad_to_deg = 57.29f;
std::array<std::string, 8> angle_names{ "N", "NE", "E", "SE", "S", "SW", "W", "NW" };
int32_t angle_verbose_deg = -1;
int32_t angle_range_index = -1;
}

float generateRandomZAngle();

#ifndef RADTODEG
#define RADTODEG 57.295f
#endif

E_BikeDemoView::E_BikeDemoView():
    minute(3), second(0), batteryTickCounter(0), batteryCharge(6),
    compassAnimationEndedCallback(this, &E_BikeDemoView::compassAnimationEndedHandler),
    compassAnimationStepCallback(this, &E_BikeDemoView::updateDirectionInformation)

{

}

void E_BikeDemoView::setupScreen()
{
    E_BikeDemoViewBase::setupScreen();

    _current_highlight = &highlightMph;

    // fill graph with a bunch of values
    for (auto i = 0; i < graph.getMaxCapacity(); i++)
    {
        while (!_elevation_sim.update())
        {
            // Update until we have a new value
        }

        graph.addDataPoint(_elevation_sim._elevation_m);
    }

    compass.setTextureMapperAnimationEndedAction(compassAnimationEndedCallback);
    compass.setTextureMapperAnimationStepAction(compassAnimationStepCallback);

    compass.setupAnimation(AnimationTextureMapper::Z_ROTATION, 3.0f, 130, 10, EasingEquations::cubicEaseInOut);
    compass.startAnimation();

    update();
}

void E_BikeDemoView::tearDownScreen()
{
    E_BikeDemoViewBase::tearDownScreen();
}

void E_BikeDemoView::updateSpeedometer(float dt)
{
    const auto use_kmh = (_unit_velocity == Units::UnitVelocity::KilometersPerHour);

    auto velocity_kmh = 0.0f;
    auto needle_angle = 0.0f;
    if (_tick_user_input_has_value && (_tick_current - _tick_user_input < ticks_disable_simulation_period))
    {
        // turn needle angle from -pi/2 .. pi/2 into a velocity from 0 to 1
        velocity_kmh = ((_needle_angle + PI_2) / PI) * velocity_max_kmh;
        _velocity_from_user_kmh = velocity_kmh;
        needle_angle = _needle_angle;
        _velocity_interpolate = 1.0f;
    }
    else
    {
        // when the simulation has expired, just linear interpolate to the value
        // the simulation is feeding to the speedometer
        velocity_kmh = (1.0f - _velocity_interpolate) * _speedometer._velocity_kmh + (_velocity_interpolate * _velocity_from_user_kmh);
        const auto velocity_normalized = use_kmh ? ((velocity_kmh / velocity_max_kmh) - 0.5f) : ((Units::kmhToMph(velocity_kmh) / velocity_max_mph) - 0.5f);
        needle_angle = velocity_normalized * PI;
        _velocity_interpolate = std::max<float>(0.0f, _velocity_interpolate - dt * 0.01f);
    }

    // map speed from 0..60 km/h or 0..40 mph
    if (_needle_update_required || fabs(_needle_angle - needle_angle) > 0.003f)
    {
        _needle_angle = needle_angle;
        needle.invalidateContent(); // invalidate the old needle range
        needle.setZAngle(_needle_angle);
        needle.invalidateContent(); // invalidate the new needle range
        _current_highlight->updateArc(-90.0f, _needle_angle * RADTODEG);
        _needle_update_required = false;
    }

    // update displayed velocity if needed
    const auto velocity_display = use_kmh ? velocity_kmh : Units::kmhToMph(velocity_kmh);
    const auto displayed_velocity = static_cast<int32_t>(velocity_display);
    if (displayed_velocity != _displayed_velocity)
    {
        _displayed_velocity = displayed_velocity;
        Unicode::snprintf(currentVelocityBuffer, 3, "%d", _displayed_velocity);
        currentVelocity.invalidate();
    }

    // update displayed total distance if needed
    const auto displayed_total_distance = use_kmh ? _speedometer._distance_total_km : Units::kmhToMph(_speedometer._distance_total_km);
    if (fabs(_displayed_total_distance - displayed_total_distance) > 0.05f)
    {
        _displayed_total_distance = displayed_total_distance;
        Unicode::snprintfFloat(totalDistanceBuffer, 5, "%.1f", displayed_total_distance);
        totalDistance.invalidate();
    }
}

void E_BikeDemoView::updateElevation()
{
    // for the graph it makes good sense to just pass the texture values straight away, leaving them at 0..255.
    // for plausibility, since our pilot is not going from 0..255m in 2 seconds, it's probably better to scale
    // the elevation down a bit to 1/5.
    graph.addDataPoint(_elevation_sim._elevation_m);

    const bool use_meters = (_unit_length == Units::UnitLength::Meters);

    Unicode::snprintf(
        elevationLabelBuffer,
        6,
        use_meters ? "+%dm" : "+%dft",
        use_meters ? static_cast<int32_t>(_elevation_sim._elevation_m) : static_cast<int32_t>(Units::metersToFeet(_elevation_sim._elevation_m))
    );

    elevationLabel.invalidate();
}

void E_BikeDemoView::updateDirectionInformation(const AnimationTextureMapper& src)
{

    // update the angle value given in degrees degrees
    auto angle_verbose_deg_tmp = static_cast<int32_t>(compass.getZAngle() * rad_to_deg) % 360;
    if (angle_verbose_deg_tmp < 0)
    {
        angle_verbose_deg_tmp += 360;
    }
    angle_verbose_deg_tmp = 360 - angle_verbose_deg_tmp;
    if (angle_verbose_deg_tmp == 360)
    {
        angle_verbose_deg_tmp = 0;
    }
    if (angle_verbose_deg_tmp != angle_verbose_deg)
    {
        angle_verbose_deg = angle_verbose_deg_tmp;
        Unicode::snprintf(rotationDegreeTextBuffer, ROTATIONDEGREETEXT_SIZE, "%d", angle_verbose_deg);
        rotationDegreeText.invalidate();
    }

    // update the angle range description if needed
    // auto angle_range_index_tmp = angle_verbose_deg / 45;
    auto angle_range_index_tmp = computeRangeIndex(static_cast<float>(angle_verbose_deg));
    if (angle_range_index_tmp != angle_range_index)
    {
        angle_range_index = angle_range_index_tmp;

        const std::string& text = angle_names[angle_range_index];
        Unicode::strncpy(rotationDirectionTextBuffer, text.c_str(), static_cast<uint16_t>(text.size() + 1));
        rotationDirectionText.invalidate();
    }
}

void E_BikeDemoView::update()
{
    // assume constant tick speed
    const auto dt = 1.0f;

    if (_elevation_sim.update())
    {
        updateElevation();
    }

    if (_speedometer.update(dt))
    {
        updateSpeedometer(dt);
    }

    _tick_current++;
}

void E_BikeDemoView::handleTickEvent()
{
    update();

    mcuLoadPercentageContainer.checkMCULoad();
    frameRateContainer.countTheFrames();

    if (batteryTickCounter == 30)
    {
        updateTheRemainingBatteryTime();
        batteryTickCounter = 0;
    }
    batteryTickCounter++;

    _tick_current++;
}

void E_BikeDemoView::updateTheRemainingBatteryTime()
{
    if (second == 0 || second == 30)
    {
        updateTheBatteryIcon();
    }

    second--;
    if (second < 0)
    {
        minute--;
        if (minute < 0)
        {
            minute = 3;
            second = 0;
            updateTextOfTimer();
            return;
        }
        second = 59;
    }
    updateTextOfTimer();
}

void E_BikeDemoView::updateTheBatteryIcon()
{
    switch (batteryCharge)
    {
    case 6:
        batteryCharge = 5;
        break;

    case 5:
        battery.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_STEPS_5OF6_ID));
        batteryCharge = 4;
        break;

    case 4:
        battery.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_STEPS_4OF6_ID));
        batteryCharge = 3;
        break;

    case 3:
        battery.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_STEPS_3OF6_ID));
        batteryCharge = 2;
        break;

    case 2:
        battery.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_STEPS_2OF6_ID));
        batteryCharge = 1;
        break;

    case 1:
        battery.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_STEPS_LOW_ID));
        batteryCharge = 0;
        break;

    case 0:
        battery.setBitmap(touchgfx::Bitmap(BITMAP_BATTERY_STEPS_FULL_ID));
        batteryCharge = 6;
        break;
    }

    battery.invalidate();
}

void E_BikeDemoView::updateTextOfTimer()
{
    Unicode::snprintf(remainingBuffer, 6, "%02d:%02d", minute, second);
    remaining.invalidate();
}

void E_BikeDemoView::handleDragEvent(const touchgfx::DragEvent& event)
{
    const auto x1 = static_cast<float>(event.getOldX());
    const auto y1 = static_cast<float>(event.getOldY());

    // disregard drag events not coming from the outer handle
    const auto vx_px = event.getOldX() - screen_center_x;
    const auto vy_px = event.getOldY() - screen_center_y;
    const auto v_length_squared = vx_px * vx_px + vy_px * vy_px;
    if (v_length_squared < outer_handle_distance_squared || vy_px > 0)
    {
        return;
    }

    const auto x2 = static_cast<float>(event.getNewX());
    const auto y2 = static_cast<float>(event.getNewY());

    const auto dx1 = x1 - speedometer_center_x;
    const auto dy1 = y1 - speedometer_center_y;
    const auto dx2 = x2 - speedometer_center_x;
    const auto dy2 = y2 - speedometer_center_y;

    const auto angle_1 = atan2(dy1, dx1);
    const auto angle_2 = atan2(dy2, dx2);
    const auto delta_angle = static_cast<float>(angle_2 - angle_1);

    // disregard jumps when user spins the handle too far
    if (static_cast<float>(fabs(delta_angle)) > PI_2)
    {
        return;
    }


    // user wants to set the speed by controlling the speedometer needle. from this point on,
    // the speed simulation is disabled and the user keeps controlling the needle until enough
    // ticks without any user-driven event have passed.
    _needle_angle += delta_angle;
    clamp(_needle_angle, PI_2, -PI_2);
    _tick_user_input = _tick_current;
    _tick_user_input_has_value = true;
    _needle_update_required = true;
}

void E_BikeDemoView::clamp(float& angle, float upper_limit, float lower_limit)
{
    if (angle > upper_limit)
    {
        angle = upper_limit;
    }
    else if (angle < lower_limit)
    {
        angle = lower_limit;
    }
}

void E_BikeDemoView::selectKmh()
{
    _unit_velocity = Units::UnitVelocity::KilometersPerHour;
    _unit_length = Units::UnitLength::Meters;
    _current_highlight = &highlightKmh;
    _needle_update_required = true;

    // gotta update the speedometer values before showing the circles
    // otherwise there'd be a frame where the velocity is inconsistent with the ui
    updateSpeedometer(1.0f);
}

void E_BikeDemoView::selectMiles()
{
    _unit_velocity = Units::UnitVelocity::MilesPerHour;
    _unit_length = Units::UnitLength::Feet;
    _current_highlight = &highlightMph;
    _needle_update_required = true;

    updateSpeedometer(1.0f);
}



float Speedometer::computeVelocity(float time) const
{
    return 30.0f + static_cast<float>(std::sin(0.01f * time)) * 20.0f;
}

bool Speedometer::update(float dt)
{
    _elapsed_time_ticks += dt;
    _velocity_kmh = computeVelocity(_elapsed_time_ticks);
    _distance_total_km += 0.001f;

    return true;
}

float Units::metersToFeet(float m)
{
    return 3.28f * m;
}

float Units::kmhToMph(float kmh)
{
    return 0.621f * kmh;
}

bool ElevationSimulation::update()
{
    bool elevationUpdated = false;
    static uint64_t count = 0;

    if (count % 2 == 0)
    {
        _elevation_m = 127.0f + (60 * sin(count * 0.014f) + 30 * sin(count * 0.035f));
        elevationUpdated = true;
    }
    count++;

    return elevationUpdated;
}

void E_BikeDemoView::compassAnimationEndedHandler(const AnimationTextureMapper& src)
{
    float newZAngle = generateRandomZAngle();

    compass.setupAnimation(AnimationTextureMapper::Z_ROTATION, newZAngle, 200, 50, EasingEquations::cubicEaseInOut);
    compass.startAnimation();

}

float generateRandomZAngle()
{
    float zAngle = static_cast<float>(Utils::randomNumberBetween(0, 360)) / 360.0f * 6.24f;
    return zAngle;
}


int32_t E_BikeDemoView::computeRangeIndex(float angle_deg)
{
    return static_cast<int32_t>(std::floor((angle_deg + 22.5f) / 45.0f)) & 7;
}
