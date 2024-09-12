#include <SDL2/SDL.h>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define A 0
#define B 1
#define X 2
#define Y 3
#define LB 9
#define RB 10

SDL_GameController *findController()
{
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        if (SDL_IsGameController(i))
        {
            return SDL_GameControllerOpen(i);
        }
    }

    return nullptr;
}

inline SDL_JoystickID getControllerInstanceID(SDL_GameController *controller)
{
    return SDL_JoystickInstanceID(
        SDL_GameControllerGetJoystick(controller));
}

void renderCross1(SDL_Surface *screenSurface, int x, int y, int w, Uint32 color)
{
    SDL_Rect rect = {x, y - w / 2, 1, w};
    SDL_FillRect(screenSurface, &rect, color);

    rect = {x - w / 2, y, w, 1};
    SDL_FillRect(screenSurface, &rect, color);
}

void renderCross(SDL_Surface *screenSurface, int x, int y, float x1, float x2)
{
    renderCross1(screenSurface, x, y, 20, SDL_MapRGB(screenSurface->format, 0x99, 0x99, 0x99));
    renderCross1(screenSurface, x + x1 * SCREEN_WIDTH * 0.2f, y + x2 * SCREEN_HEIGHT * 0.2f,
                 10, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xAA));
}

int main(int argc, char *args[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "sdl controller",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == nullptr)
    {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    bool quit = false;
    SDL_Surface *screenSurface = SDL_GetWindowSurface(window);

    // Note that this code only allows a single active controller at the time, but it would
    // be easy to modify to keep track of multiple controllers.

    // SDL_CONTROLLERDEVICEADDED is not triggered when starting up, so check for controllers now.
    SDL_GameController *controller = findController();

    SDL_Event event;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            SDL_FillRect(screenSurface, nullptr, 0x00ff00 * event.cbutton.state);
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_CONTROLLERDEVICEADDED:
                if (!controller)
                {
                    controller = SDL_GameControllerOpen(event.cdevice.which);
                }
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                if (controller && event.cdevice.which == getControllerInstanceID(controller))
                {
                    SDL_GameControllerClose(controller);
                    controller = findController();
                }
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                if (controller && event.cdevice.which == getControllerInstanceID(controller))
                {
                    switch (event.cbutton.button)
                    {
                    case X:
                        std::cerr << "X pressed!" << std::endl;
                        break;
                    case Y:
                        std::cerr << "Y pressed!" << std::endl;
                        break;
                    case A:
                        std::cerr << "A pressed!" << std::endl;
                        break;
                    case B:
                        std::cerr << "B pressed!" << std::endl;
                        break;
                    case LB:
                        std::cerr << "LB pressed!" << std::endl;
                        break;
                    case RB:
                        std::cerr << "RB pressed!" << std::endl;
                        break;
                    }
                }
                break;
            }
        }

        // bool pressed = controller && SDL_GameControllerGetButton(controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X);

        // SDL_FillRect(screenSurface, nullptr,
        //              SDL_MapRGB(screenSurface->format, 0x33, 0x33, pressed ? 0xFF : 0x33));

        if (controller)
        {
            float x = (float)SDL_GameControllerGetAxis(controller,
                                                       SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) /
                      (float)INT16_MAX;
            float y = (float)SDL_GameControllerGetAxis(controller,
                                                       SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) /
                      (float)INT16_MAX;
            renderCross(screenSurface, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, x, y);

            x = (float)SDL_GameControllerGetAxis(controller,
                                                 SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) /
                (float)INT16_MAX;
            y = (float)SDL_GameControllerGetAxis(controller,
                                                 SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY) /
                (float)INT16_MAX;
            renderCross(screenSurface, 3 * SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, x, y);
        }

        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
