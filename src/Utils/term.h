#pragma once

#include <map>
#include <string>
#include "lynx_common.h"

namespace Utils {

    enum ColorType {
        // Foreground colors
        FG_BLACK,
        FG_RED,
        FG_GREEN,
        FG_ORANGE,
        FG_BLUE,
        FG_MAGENTA,
        FG_CYAN,
        FG_LIGHT_GRAY,
        FG_WHITE,
        
        // Background Colors
        BG_BLACK,
        BG_RED,
        BG_GREEN,
        BG_ORANGE,
        BG_BLUE,
        BG_MAGENTA,
        BG_CYAN,
        BG_LIGHT_GRAY,
        BG_WHITE,

        // Special colours
        FG_LIGHT_RED

    };

    std::string GetColorString(ColorType colour);

}
