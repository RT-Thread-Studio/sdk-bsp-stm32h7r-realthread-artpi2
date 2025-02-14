#ifndef E_BIKEDEMOVIEW_HPP
#define E_BIKEDEMOVIEW_HPP

#include <gui_generated/e_bikedemo_screen/E_BikeDemoViewBase.hpp>
#include <gui/e_bikedemo_screen/E_BikeDemoPresenter.hpp>

#include <array>
#include <cstdint>

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
    float _elapsed_time_ticks = 0.0f;
    float _altitude_m = 0.0f;
    float _velocity_kmh = 0.0f;
    float _velocity_prev_kmh = 0.0f;
    float _distance_total_km = 0.0f;

    float computeVelocity(float time) const;
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

    bool update();
};

class E_BikeDemoView : public E_BikeDemoViewBase
{
public:
    E_BikeDemoView();
    virtual ~E_BikeDemoView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void selectKmh() override;
    void selectMiles() override;

protected:
    void handleTickEvent() override;
    void handleDragEvent(const touchgfx::DragEvent& event);

private:
    void update();
    void updateElevation();
    void updateSpeedometer(float dt);
    void clamp(float& angle, float upper_limit, float lower_limit);

    int32_t computeRangeIndex(float angle_deg);

    Speedometer _speedometer;
    ElevationSimulation _elevation_sim;
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

    int minute;
    int second;
    int batteryTickCounter;
    int batteryCharge;
    void updateTheRemainingBatteryTime();
    void updateTextOfTimer();
    void updateTheBatteryIcon();

    Callback<E_BikeDemoView, const AnimationTextureMapper&> compassAnimationEndedCallback;
    Callback<E_BikeDemoView, const AnimationTextureMapper&> compassAnimationStepCallback;
    void compassAnimationEndedHandler(const AnimationTextureMapper& src);
    void updateDirectionInformation(const AnimationTextureMapper& src);
};

#endif // E_BIKEDEMOVIEW_HPP
