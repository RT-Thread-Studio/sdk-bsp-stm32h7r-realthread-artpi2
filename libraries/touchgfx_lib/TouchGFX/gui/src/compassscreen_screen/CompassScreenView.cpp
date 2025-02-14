#include <gui/compassscreen_screen/CompassScreenView.hpp>

// angle range indices
//
//       0      45     90    135    180    225    270    315    360
//    <--+--><--+--><--+--><--+--><--+--><--+--><--+--><--+--><--+-->
//       N      NE     E      SE     S      SW     W      NW     N
//       |      |      |      |      |      |      |      |      |
//       0      1      2      3      4      5      6      7      0
//
//   0: 337.5 ..  22.5 ->  N
//   1:  22.5 ..  67.5 -> NE
//   2:  67.5 .. 112.5 ->  E
//   3: 112.5 .. 157.5 -> SE
//   4: 157.5 .. 202.5 ->  S
//   5: 202.5 .. 247.5 -> SW
//   6: 247.5 .. 292.5 ->  W
//   7: 292.5 .. 337.5 -> NW
int32_t CompassScreenView::computeRangeIndex(float angle_deg)
{
    return static_cast<int32_t>(std::floor((angle_deg + 22.5f) / 45.0f)) & 7;
}


CompassScreenView::CompassScreenView() :
    _wheel{ },
    _tick(0),
    tick(0),
    angle_verbose_deg(-1),
    angle_range_index(-1),
    initialized(false),
    last_drag_delta(0.0f)
{

}

void CompassScreenView::setupScreen()
{
    CompassScreenViewBase::setupScreen();
    _wheel._dirty = true;
    updateView();
}

void CompassScreenView::handleDragEvent(const touchgfx::DragEvent& event)
{
    if ((event.getNewX() > 252 &&  event.getNewX() < 672) && (event.getNewY() > 30 &&  event.getNewY() < 450))
    {
        const float x1 = static_cast<float>(event.getOldX());
        const float y1 = static_cast<float>(event.getOldY());

        const float x2 = static_cast<float>(event.getNewX());
        const float y2 = static_cast<float>(event.getNewY());

        const float dx1 = x1 - wheel_center_x;
        const float dy1 = y1 - wheel_center_y;

        const float dx2 = x2 - wheel_center_x;
        const float dy2 = y2 - wheel_center_y;

        const float angle_1 = atan2(dy1, dx1);
        const float angle_2 = atan2(dy2, dx2);

        const float delta_angle = angle_2 - angle_1;

        last_drag_delta = static_cast<float>(delta_angle);

        _wheel._velocity = 0.0f;
        _wheel._angle_rad += static_cast<float>(delta_angle);
        _wheel._dirty = true;
    }
}

// for gesture events, just use the angle from the last drag movement is used and the velocity is applied
void CompassScreenView::handleGestureEvent(const touchgfx::GestureEvent& event)
{
    if ((event.getX() > 252 &&  event.getX() < 672) && (event.getY() > 30 &&  event.getY() < 450))
    {
        const float velocity = static_cast<float>(fabs(static_cast<float>(event.getVelocity()) * 0.01f));
        _wheel._velocity = (last_drag_delta < 0.0f ? -1.0f : 1.0f) * velocity;
    }
}


void CompassScreenView::handleTickEvent()
{
    updateView();

    frameRateContainer.countTheFrames();
    mcuLoadPercentageContainer.checkMCULoad();

    tick++;
}

void CompassScreenView::updateView()
{
    const float dt = static_cast<float>(tick - _tick);
    _tick = tick;

    if (_wheel.update(dt) || !initialized)
    {
        initialized = true;

        wheel.setZAngle(_wheel._angle_rad);
        wheel.invalidate();

        // update the angle value given in degrees degrees
        int32_t angle_verbose_deg_tmp = static_cast<int32_t>(_wheel._angle_rad * rad_to_deg) % 360;
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
            Unicode::snprintf(angleNumericBuffer, 4, "%d", angle_verbose_deg);
            angleNumeric.invalidate();
        }

        // update the angle range description if needed
        int32_t angle_range_index_tmp = computeRangeIndex(static_cast<float>(angle_verbose_deg));
        if (angle_range_index_tmp != angle_range_index)
        {
            angle_range_index = angle_range_index_tmp;

            const std::string& text = angle_names[angle_range_index];
            Unicode::strncpy(angleVerboseBuffer, text.c_str(), static_cast<uint16_t>(text.size() + 1));
            angleVerbose.invalidate();
        }
    }
}

bool CompassScreenView::Wheel::update(float dt)
{
    // apply friction to velocity
    _velocity *= (dt * wheel_friction);

    // apply velocity to rotation
    const float angle_rad = _angle_rad;
    _angle_rad += (dt * _velocity);

    if (_dirty)
    {
        _dirty = false;
        return true;
    }

    return (fabs(_angle_rad - angle_rad) > update_epsilon);
}

void CompassScreenView::tearDownScreen()
{
    CompassScreenViewBase::tearDownScreen();
}
