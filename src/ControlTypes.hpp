#ifndef __CONTROL_TYPES_HPP__
#define __CONTROL_TYPES_HPP__

#include "Templates.hpp"

namespace DarkEmperor::Controls
{
    enum class Type
    {
        NONE = -1,
        SCROLL_UP,
        SCROLL_DOWN,
        SCROLL_LEFT,
        SCROLL_RIGHT,
        MAP_UP,
        MAP_DOWN,
        MAP_LEFT,
        MAP_RIGHT,
        BACK,
        CONFIRM,
        CANCEL,
        ROLL,
        PLAYER,
        KINGDOM,
        LOCATION,
        MOVE,
        SPELLS,
        NEW,
        LOAD,
        SAVE,
        EXIT,
        GAME,
        ABOUT,
        NEXT,
        CENTER,
        SELECT,
        MONEY,
        STATUS,
        UP,
        DOWN,
        LEFT,
        RIGHT,
        MENU
    };
}

#endif
