#include "SDLWrapper.h"
using namespace SDL;

int main(int argc, char* argv[]) {
	static const Uint32 fps = 60;

	try {

		Library library(SDL_INIT_EVERYTHING);

		Window window("First", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			600, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		Image image("whale-watch.jpg");
		SDL_Rect src = { 0,0,image.width(), image.height() };
		SDL_Rect dst = { 0,0,image.width(), image.height() };

		SDL_Surface* screen = ::SDL_GetWindowSurface(window);
		if (screen == nullptr) {
			return 3;
		}

		Uint32 previous = ::SDL_GetTicks() + 1000/fps;
		SDL_Event event;
		bool quit = false;
		bool fullscreen = false;
		while (!quit) {
			while (::SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) quit = true;
				if (event.type == SDL_KEYDOWN) {
					if (fullscreen)
						::SDL_SetWindowFullscreen(window, 0);
					else
						::SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

					fullscreen = !fullscreen;
				}
			}

			while (!SDL_TICKS_PASSED(::SDL_GetTicks(), previous)) {
				::SDL_Delay(previous - ::SDL_GetTicks());
			}

			// draw screen.
			screen = ::SDL_GetWindowSurface(window);

			::SDL_BlitSurface(image, NULL, screen, NULL);

			::SDL_UpdateWindowSurface(window);

		}

	}
	catch (const Error& err) {
		err.what();
	}

	return 0;
}