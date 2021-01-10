#include "Util/GlobalConfig.h"

using namespace VanUtil;

// settings
const static uint16_t SCR_WIDTH  = 800;
const static uint16_t SCR_HEIGHT = 600;

GlobalConfig::GlobalConfig() : screenWidth_(SCR_WIDTH), screenHeight_(SCR_HEIGHT)
{
}

GlobalConfig::~GlobalConfig()
{
}

GlobalConfig &GlobalConfig::instance()
{
    static GlobalConfig ins;
    return ins;
}
