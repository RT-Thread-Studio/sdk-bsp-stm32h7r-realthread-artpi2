class Constants
{
public:
    Constants() {}
    ~Constants() {}

    static constexpr float PI = 3.14159265358979323846f;
    static constexpr float PI_HALF = PI / 2;
    static constexpr float PI_QUARTER = PI / 4;
    static constexpr float PI_THREE_QUARTER = PI / 4 * 3;
    static constexpr float PI_SCALE = 1 / PI;

    //Main Screen//
    static constexpr int SUBDEMO_NAME_TEXT_START_X = 105;
    static constexpr int SUBDEMO_NAME_TEXT_END_X = 145;
    static constexpr int SUBDEMO_NAME_TEXT_MOVE_ANIMATION_DURATION = 60;
    static constexpr int SUBDEMO_NAME_TEXT_FADE_ANIMATION_DURATION = 40;
    static constexpr int MAIN_EXIT_BOUNCE_ANIMATION_DURATION = 50;
    static constexpr int MAIN_EXIT_SCALE_ANIMATION_DURATION = 100;

};
