#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <iostream>

#include "Graphics.hpp"

#if __APPLE__
#define CMD_BUTTON KMOD_GUI
#else
#define CMD_BUTTON KMOD_CTRL
#endif

namespace DarkEmperor::Input
{
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
}

#endif
