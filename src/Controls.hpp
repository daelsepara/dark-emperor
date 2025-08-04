#ifndef __CONTROLS_HPP__
#define __CONTROLS_HPP__

#include <SDL.h>

#include "ControlTypes.hpp"
#include "Types.hpp"

namespace DarkEmperor::Controls
{
    const int NotFound = -1;

    int Default = -1;

    struct Set
    {
        // ID
        int Me = -1;

        // ID of control to the left of this control, equal to ID if none
        int Left = -1;

        // ID of control to the right of this control, equal to ID if none
        int Right = -1;

        // ID of control above this control, equal to ID if none
        int Up = -1;

        // ID of control below this control, equal to ID if none
        int Down = -1;
    } IdSet;

    class Base
    {
    public:
        // type of control. used in distinguishing selected control in user input
        Controls::Type Type = Controls::Type::NONE;

        Set Id = {-1, -1, -1, -1, -1};

        // location on the screen (part of this control's hitbox definition)
        Point Location = Point(0, 0);

        // dimensions of this control's hitbox
        Point Dimensions = Point(0, 0);

        // color of border when hightlighted
        Uint32 Highlight;

        // control is on the map
        bool OnMap = false;

        // location on the map (hex)
        Point Map = Point(-1, -1);

        // vertices of the hex highlight (used with OnMap and Map)
        Points Hex = {};
    };

    // user input base class
    class User
    {
    public:
        // type of the control currently in focus
        Controls::Type Type = Controls::Type::NONE;

        // ID of the control currently in focus, -1 if none
        int Current = -1;

        // the control has been explicitly selected
        // i.e. through a mouse button click, pressing a button on the gamepad,
        // or via the RETURN key (on the keyboard)
        bool Selected = false;

        // a scroll up event
        bool Up = false;

        // a scroll down event
        bool Down = false;

        // control is currently being held down (e.g. mouse left button held over the control, etc.)
        bool Hold = false;

        // quit/exit/terminate event
        bool Quit = false;

        // blink curser
        bool Blink = false;

        // text event
        bool Text = false;

        // flag to indicate a re-render text
        bool RefreshText = false;

        // input text
        std::string TextInput = std::string();

        // Character limit
        int TextLimit = 20;

        User(Controls::Type type,
             int current,
             bool selected,
             bool up,
             bool down,
             bool hold) : Type(type),
                          Current(current),
                          Selected(selected),
                          Up(up),
                          Down(down),
                          Hold(hold) {}

        User(int current,
             bool selected,
             bool up,
             bool down) : Current(current), Selected(selected), Up(up), Down(down) {}

        User(int current, bool selected) : Current(current), Selected(selected) {}

        User(int current) : Current(current) {}

        User() { this->Current = Controls::Default; }

        void SetText(const char *text)
        {
            this->TextInput = std::string(text);
        }

        void SetText(std::string text)
        {
            this->TextInput = text;
        }

        void ClearText()
        {
            this->TextInput.clear();
        }

        void Clear()
        {
            this->Type = Controls::Type::NONE;

            this->Selected = false;

            this->Current = -1;
        }
    };
}

#endif