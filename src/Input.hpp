#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <iostream>

#include "Controls.hpp"
#include "Graphics.hpp"

#if __APPLE__
#define CMD_BUTTON KMOD_GUI
#else
#define CMD_BUTTON KMOD_CTRL
#endif

namespace DarkEmperor::Input
{
    const int StandardDelay = 100;

    // initialize any connected gamepads
    int InitializeGamePads()
    {
        if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 1)
        {
            if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0)
            {
                std::cerr << "SDL could not initialize gamecontroller! SDL_Error: " << SDL_GetError() << std::endl;
            }
        }

        auto n_joysticks = SDL_NumJoysticks();

        auto n_gamepads = 0;

        // count how many controllers there are
        for (auto i = 0; i < n_joysticks; i++)
        {
            if (SDL_IsGameController(i))
            {
                n_gamepads++;
            }
        }

        // If we have some controllers attached
        if (n_gamepads > 0)
        {
            for (auto i = 0; i < n_gamepads; i++)
            {
                // Open the controller and add it to our list
                auto pad = SDL_GameControllerOpen(i);

                if (SDL_GameControllerGetAttached(pad) != SDL_TRUE)
                {
                    std::cerr << "Game pad not attached! SDL_Error: " << SDL_GetError() << std::endl;
                }
            }

            SDL_GameControllerEventState(SDL_ENABLE);
        }

        return n_gamepads;
    }

    // wait for button (gamepad/mouse) click or the return key
    void WaitForNext(Graphics::Base &graphics, Scene &scene)
    {
        SDL_Event result;

        while (true)
        {
            Graphics::RenderNow(graphics, scene);

            SDL_WaitEventTimeout(&result, Input::StandardDelay);

            if (result.type == SDL_QUIT)
            {
                break;
            }
            else if (result.type == SDL_WINDOWEVENT)
            {
                Graphics::HandleWindowEvent(result, graphics);
            }
            else if (result.type == SDL_CONTROLLERDEVICEADDED)
            {
                Input::InitializeGamePads();
            }
            else if (result.type == SDL_KEYDOWN)
            {
                if (result.key.keysym.sym == SDLK_KP_ENTER || result.key.keysym.sym == SDLK_RETURN || result.key.keysym.sym == SDLK_RETURN2)
                {
                    break;
                }
            }
            else if (result.type == SDL_CONTROLLERBUTTONUP)
            {
                if (result.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                {
                    break;
                }
            }
            else if (result.type == SDL_MOUSEBUTTONUP && result.button.button == SDL_BUTTON_LEFT)
            {
                break;
            }

            SDL_FlushEvent(result.type);
        }
    }

    // enable text input events
    void StartTextInput()
    {
        SDL_StartTextInput();
    }

    // disable text input events
    void StopTextInput()
    {
        SDL_StopTextInput();
    }

    Controls::User WaitForText(Graphics::Base &graphics, Scenes scenes, Controls::Collection &controls, Controls::User input, bool blur = true, int delay = Input::StandardDelay)
    {
        Graphics::RenderNow(graphics, scenes, controls, input, blur);

        SDL_Event result;

        if (delay > 0)
        {
            SDL_WaitEventTimeout(&result, delay);
        }
        else
        {
            SDL_PollEvent(&result);
        }

        input.Selected = false;

        input.RefreshText = false;

        if (result.type == SDL_KEYDOWN)
        {
            if (result.key.keysym.sym == SDLK_BACKSPACE && input.TextInput.size() > 0)
            {
                input.TextInput.pop_back();

                input.RefreshText = true;
            }
            else if (result.key.keysym.sym == SDLK_KP_ENTER || result.key.keysym.sym == SDLK_RETURN || result.key.keysym.sym == SDLK_RETURN2)
            {
                input.Selected = true;
            }
            else if (result.key.keysym.sym == SDLK_c && SDL_GetModState() & CMD_BUTTON)
            {
                if (input.TextInput.size() > 0)
                {
                    // copy text to clipboard
                    SDL_SetClipboardText(input.TextInput.c_str());
                }
            }
            else if (result.key.keysym.sym == SDLK_v && SDL_GetModState() & CMD_BUTTON)
            {
                // copy text from clipboard
                auto clipboard_text = SDL_GetClipboardText();

                if (std::strlen(clipboard_text) > 0)
                {
                    auto text = Utilities::CleanString(std::string(clipboard_text), "\n\r");

                    input.TextInput = (input.TextInput + text).substr(0, input.TextLimit);

                    input.RefreshText = true;
                }

                SDL_free(clipboard_text);
            }
        }
        else if (result.type == SDL_TEXTINPUT)
        {
            // not copy or pasting
            if (!(SDL_GetModState() & CMD_BUTTON && (result.text.text[0] == 'c' || result.text.text[0] == 'C' || result.text.text[0] == 'v' || result.text.text[0] == 'V')))
            {
                // Append character
                if (input.TextInput.size() < input.TextLimit)
                {
                    input.TextInput += result.text.text;

                    input.RefreshText = true;
                }
            }
        }

        SDL_FlushEvent(result.type);

        return input;
    }

    // render all scenes and wait for input from specified controls set
    Controls::User WaitForInput(Graphics::Base &graphics, Scenes scenes, Controls::Collection &controls, Controls::User &input, bool blur = true, int delay = Input::StandardDelay)
    {
        if (input.Text)
        {
            return Input::WaitForText(graphics, scenes, controls, input, blur, delay);
        }

        Graphics::RenderNow(graphics, scenes, controls, input, blur);

        SDL_Event result;

        auto sensitivity = 32000;

        if (delay > 0)
        {
            SDL_WaitEventTimeout(&result, delay);
        }
        else
        {
            SDL_PollEvent(&result);
        }

        input.Selected = false;

        input.Up = false;

        input.Down = false;

        if (result.type == SDL_QUIT)
        {
            input.Quit = true;
        }
        else if (result.type == SDL_WINDOWEVENT)
        {
            Graphics::HandleWindowEvent(result, graphics);
        }
        else if (result.type == SDL_CONTROLLERDEVICEADDED)
        {
            Input::InitializeGamePads();
        }
        else if (result.type == SDL_KEYDOWN && !controls.empty())
        {
            if (result.key.keysym.sym == SDLK_PAGEUP)
            {
                input.Up = true;

                input.Current = -1;
            }
            else if (result.key.keysym.sym == SDLK_PAGEDOWN)
            {
                input.Down = true;

                input.Current = -1;
            }
            else if (input.Current < 0)
            {
                input.Current = controls[0].Id.Me;
            }
            else if (result.key.keysym.sym == SDLK_TAB || result.key.keysym.sym == SDLK_KP_TAB || result.key.keysym.sym == SDL_SCANCODE_KP_TAB)
            {
                if (input.Current < 0)
                {
                    input.Current = controls[0].Id.Me;
                }
                else if (input.Current == controls.size() - 1)
                {
                    input.Current = controls[0].Id.Me;
                }
                else if (input.Current >= 0 && input.Current < controls.size() - 1)
                {
                    input.Current = controls[input.Current + 1].Id.Me;
                }
            }
            else if (input.Current >= 0 && input.Current < controls.size())
            {
                if (result.key.keysym.sym == SDLK_LEFT || result.key.keysym.sym == SDLK_KP_4)
                {
                    input.Current = controls[input.Current].Id.Left;
                }
                else if (result.key.keysym.sym == SDLK_RIGHT || result.key.keysym.sym == SDLK_KP_6)
                {
                    input.Current = controls[input.Current].Id.Right;
                }
                else if (result.key.keysym.sym == SDLK_UP || result.key.keysym.sym == SDLK_KP_8)
                {
                    input.Current = controls[input.Current].Id.Up;
                }
                else if (result.key.keysym.sym == SDLK_DOWN || result.key.keysym.sym == SDLK_KP_2)
                {
                    input.Current = controls[input.Current].Id.Down;
                }
                else if (result.key.keysym.sym == SDLK_KP_ENTER || result.key.keysym.sym == SDLK_RETURN || result.key.keysym.sym == SDLK_RETURN2)
                {
                    input.Selected = true;
                }
                else if (result.key.keysym.sym == SDLK_ESCAPE)
                {
                    auto escape = Controls::Find(controls, Controls::Type::BACK);

                    auto cancel = Controls::Find(controls, Controls::Type::CANCEL);

                    auto exit = Controls::Find(controls, Controls::Type::EXIT);

                    auto confirm = Controls::Find(controls, Controls::Type::CONFIRM);

                    if (escape != -1)
                    {
                        input.Current = escape;
                    }
                    else if (cancel != -1)
                    {
                        input.Current = cancel;
                    }
                    else if (exit != -1)
                    {
                        input.Current = exit;
                    }
                    else if (confirm != -1 && escape == -1 && cancel == -1 && exit == -1)
                    {
                        input.Current = confirm;
                    }

                    if (escape != -1 || cancel != -1 || exit != -1 || confirm != -1)
                    {
                        input.Selected = true;
                    }
                }
                else if (result.key.keysym.sym == SDLK_y || result.key.keysym.sym == SDLK_n)
                {
                    auto confirm = Controls::Find(controls, Controls::Type::CONFIRM);

                    auto cancel = Controls::Find(controls, Controls::Type::CANCEL);

                    if (confirm != -1 && cancel != -1)
                    {
                        input.Current = result.key.keysym.sym == SDLK_y ? confirm : cancel;

                        input.Selected = true;
                    }
                }
            }
        }
        else if (result.type == SDL_CONTROLLERAXISMOTION && !controls.empty())
        {
            if (result.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
            {
                if (input.Current < 0)
                {
                    input.Current = controls[0].Id.Me;
                }
                else if (result.caxis.value < -sensitivity)
                {
                    if (input.Current >= 0 && input.Current < controls.size())
                    {
                        input.Current = controls[input.Current].Id.Left;
                    }
                }
                else if (result.caxis.value > sensitivity)
                {
                    if (input.Current >= 0 && input.Current < controls.size())
                    {
                        input.Current = controls[input.Current].Id.Right;
                    }
                }
            }
            else if (result.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
            {
                if (input.Current < 0)
                {
                    input.Current = controls[0].Id.Me;
                }
                else if (result.caxis.value < -sensitivity)
                {
                    if (input.Current >= 0 && input.Current < controls.size())
                    {
                        input.Current = controls[input.Current].Id.Up;
                    }
                }
                else if (result.caxis.value > sensitivity)
                {
                    if (input.Current >= 0 && input.Current < controls.size())
                    {
                        input.Current = controls[input.Current].Id.Down;
                    }
                }
            }
        }
        else if (result.type == SDL_CONTROLLERBUTTONDOWN && result.cbutton.button == SDL_CONTROLLER_BUTTON_X && !controls.empty())
        {
            input.Hold = true;

            if (input.Current >= 0 && input.Current < controls.size() && (controls[input.Current].Type == Controls::Type::SCROLL_UP || controls[input.Current].Type == Controls::Type::SCROLL_DOWN))
            {
                input.Selected = true;
            }
        }
        else if (result.type == SDL_CONTROLLERBUTTONUP && !controls.empty())
        {
            input.Selected = false;

            input.Hold = false;

            if (input.Current < 0)
            {
                input.Current = controls[0].Id.Me;
            }
            else if (input.Current >= 0 && input.Current < controls.size())
            {
                if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                {
                    input.Current = controls[input.Current].Id.Left;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                {
                    input.Current = controls[input.Current].Id.Right;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
                {
                    input.Current = controls[input.Current].Id.Up;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
                {
                    input.Current = controls[input.Current].Id.Down;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_X)
                {
                    input.Selected = true;
                }
            }
        }
        else if (result.type == SDL_MOUSEMOTION)
        {
            input.Hold = false;

            input.Current = -1;

            for (auto &control : controls)
            {
                if (result.motion.x >= control.Location.X && result.motion.x <= control.Location.X + control.Dimensions.X - 1 && result.motion.y >= control.Location.Y && result.motion.y <= control.Location.Y + control.Dimensions.Y - 1)
                {
                    input.Current = control.Id.Me;
                }
            }
        }
        else if (result.type == SDL_MOUSEBUTTONDOWN && result.button.button == SDL_BUTTON_LEFT)
        {
            input.Hold = true;

            if (input.Current >= 0 && input.Current < controls.size() && (controls[input.Current].Type == Controls::Type::SCROLL_UP || controls[input.Current].Type == Controls::Type::SCROLL_DOWN))
            {
                input.Selected = true;
            }
        }
        else if (result.type == SDL_MOUSEBUTTONUP && result.button.button == SDL_BUTTON_LEFT)
        {
            if (input.Hold)
            {
                input.Hold = false;

                if (input.Current >= 0 && input.Current < controls.size())
                {
                    input.Selected = true;
                }
            }
        }
        else if (result.type == SDL_MOUSEWHEEL)
        {
            input.Current = -1;

            input.Selected = false;

            if (result.wheel.y < 0 || result.wheel.x < 0)
            {
                input.Down = true;
            }
            else
            {
                input.Up = true;
            }
        }
        else if (input.Hold)
        {
            if (input.Current >= 0 && input.Hold < controls.size() && (controls[input.Current].Type == Controls::Type::SCROLL_UP || controls[input.Current].Type == Controls::Type::SCROLL_DOWN))
            {
                if (controls[input.Current].Type == Controls::Type::SCROLL_UP)
                {
                    input.Up = true;
                }
                else if (controls[input.Current].Type == Controls::Type::SCROLL_DOWN)
                {
                    input.Down = true;
                }

                SDL_Delay(Input::StandardDelay);
            }
        }

        if (input.Current >= 0 && input.Current < controls.size() && !input.Up && !input.Down)
        {
            input.Type = controls[input.Current].Type;
        }
        else
        {
            input.Type = Controls::Type::NONE;
        }

        SDL_FlushEvent(result.type);

        return input;
    }

    // render scenes then wait for user input
    Controls::User WaitForInput(Graphics::Base &graphics, Scenes scenes, Controls::User input, bool blur = true, int delay = Input::StandardDelay)
    {
        auto &front = scenes.back().get();

        return Input::WaitForInput(graphics, scenes, front.Controls, input, blur, delay);
    }

    // render background and overlay then wait for user input
    Controls::User WaitForInput(Graphics::Base &graphics, Scene &background, Scene &overlay, Controls::User input, bool is_dialog = false, bool blur = true, int delay = Input::StandardDelay)
    {
        auto &controls = is_dialog ? overlay.Controls : background.Controls;

        return Input::WaitForInput(graphics, {background, overlay}, controls, input, blur, delay);
    }

    // render scene then wait for user input
    Controls::User WaitForInput(Graphics::Base &graphics, Scene &overlay, Controls::User input)
    {
        auto background = Scene();

        return Input::WaitForInput(graphics, background, overlay, input, true);
    }

    // check inputs
    bool Check(Controls::User &input)
    {
        return (input.Selected && (input.Type != Controls::Type::NONE) && !input.Hold);
    }

    // check inputs including scrolling
    bool Validate(Controls::User &input)
    {
        return (Input::Check(input) || input.Up || input.Down);
    }
}

#endif
