#ifndef E_BIKESCREENVIEW_HPP
#define E_BIKESCREENVIEW_HPP

#include <gui_generated/e_bikescreen_screen/E_BikeScreenViewBase.hpp>
#include <gui/e_bikescreen_screen/E_BikeScreenPresenter.hpp>

#include <array>
#include <cstdint>
#include <deque>

namespace Units
{

enum class UnitVelocity
{
    KilometersPerHour,
    MilesPerHour
};

enum class UnitLength
{
    Meters,
    Feet
};

float metersToFeet(float m);
float kmhToMph(float kmh);
}

struct Speedometer
{
    enum class Mode
    {
        Max,
        Sport,
        Standard,
        Off,
    };

    Mode _mode = Mode::Standard;

    float _elapsed_time_ticks = 0.0f;
    float _altitude_m = 0.0f;
    float _velocity_kmh = 0.0f;
    float _velocity_prev_kmh = 0.0f;
    float _distance_total_km = 0.0f;

    float computeVelocity(float time) const;
    bool update(float dt);
};

struct Battery
{
    float _elapsed_time_ticks = 0.0f;
    int32_t _battery_state_step = 6;
    float _battery_state = 1.0f;
    int32_t _remaining_s = 0;

    bool update(float dt);
};


/*! \brief Elevation Simulation
 *         Fetches an elevation point from a heightmap
 *
 *    The idea is to pick a random scanline from a heightmap texture, iterate through it,
 *    then pick another scanline from that texture. Each scanline is darkened at its front
 *    and ending so the transitions are smooth.
 */
struct ElevationSimulation
{
    float _elevation_m = 0.0f;

    bool update(float dt);
};


class E_BikeScreenView : public E_BikeScreenViewBase
{
public:
    E_BikeScreenView();
    virtual ~E_BikeScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void selectKmh() override;
    void selectMiles() override;

protected:
    void handleTickEvent() override;
    void handleDragEvent(const touchgfx::DragEvent& event);

private:
    void update();
    void updateBattery();
    void updateElevation();
    void updateSpeedometer(float dt);
    void clamp(float& angle, float upper_limit, float lower_limit);
    int32_t computeRangeIndex(float angle_deg);

    Battery _battery;
    Speedometer _speedometer;
    ElevationSimulation _elevation_sim;
    std::array<touchgfx::Image*, 6> _battery_images;
    touchgfx::Circle* _current_highlight = nullptr;
    float _needle_angle = 0.0f;
    float _velocity_from_user_kmh = 0.0f;
    float _velocity_interpolate = 0.0f;
    int32_t _displayed_velocity = 0;
    float _displayed_total_distance = 0.0f;

    Units::UnitVelocity _unit_velocity = Units::UnitVelocity::MilesPerHour;
    Units::UnitLength _unit_length = Units::UnitLength::Feet;

    uint32_t _tick_current = 0;
    uint32_t _tick_previous = 0;
    uint32_t _tick_user_input = 0;            //!< that point in time when the user starts dragging the needle
    bool _tick_user_input_has_value = false;  //!< placeholder for std::optional
    bool _needle_update_required = false;     //!< \c true if the needle requires an arc update

    Callback<E_BikeScreenView, const AnimationTextureMapper&> compassAnimationEndedCallback;
    Callback<E_BikeScreenView, const AnimationTextureMapper&> compassAnimationStepCallback;
    void compassAnimationEndedHandler(const AnimationTextureMapper& src);
    void updateDirectionInformation(const AnimationTextureMapper& src);

};

#endif // E_BIKESCREENVIEW_HPP
