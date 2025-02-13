#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

enum SubDemo
{
    DEMO_1 = 0,
    DEMO_2,
    DEMO_3,
    DEMO_4,
    DEMO_5,
    DEMO_6,
    NUMBER_OF_SUB_DEMOS
};

static const uint16_t subDemoNames[] = \
{
    T_DEMO1, \
    T_DEMO2, \
    T_DEMO3, \
    T_DEMO4, \
    T_DEMO5, \
    T_DEMO6
};

static const uint16_t subDemoBitmapIds[] = \
{
    BITMAP_ST_FACTORY_00000_ID, \
    BITMAP_TRANSTION_00000_ID, \
    BITMAP_COMPASS_00000_ID, \
    BITMAP_SPEEDOMETER_00000_ID, \
    BITMAP_INFO_00000_ID, \
    BITMAP_SVG_00000_ID
};
