#pragma warning(disable:4290)

#include <SDL.h>
#include <SDL_main.h>
#include <string>
#include <exception>

class SDLException : public std::exception {
public :
	SDLException() : exception(), msg(::SDL_GetError()) {}
	SDLException(const std::string& m) : exception(), msg(m) {}
	virtual ~SDLException() throw() {}
	virtual const char* what() const throw() {
		return msg.c_str();
	}
private :
	std::string msg;
};

class SDL {
public :
	SDL(Uint32 flags = 0) throw(SDLException) {
		if (::SDL_Init(flags) != 0)
			throw SDLException();
	}
	virtual ~SDL() {
		::SDL_Quit();
	}
};

int main(int argc, char* argv[]) {

	SDL sdl(SDL_INIT_EVERYTHING);

	SDL_Window* window = ::SDL_CreateWindow("First",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		600, 480, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		return 2;
	}

	SDL_Surface* screen = ::SDL_GetWindowSurface(window);

	::SDL_FillRect(screen, nullptr,
		SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

	::SDL_UpdateWindowSurface(window);

	SDL_Event event;
	bool quit = false;
	while (!quit) {
		while (::SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) quit = true;
		}
	}

	::SDL_DestroyWindow(window);

	return 0;
}