#pragma warning(disable:4290)

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_image.h>
#include <string>
#include <exception>

class SDLError : public std::exception {
public :
	SDLError() : exception(), msg(::SDL_GetError()) {}
	SDLError(const std::string& m) : exception(), msg(m) {}
	virtual ~SDLError() throw() {}
	virtual const char* what() const throw() {
		return msg.c_str();
	}
private :
	std::string msg;
};

class SDL {
public :
	SDL(Uint32 flags = 0) throw(SDLError) {
		if (::SDL_Init(flags) != 0)
			throw SDLError();
	}
	virtual ~SDL() {
		::SDL_Quit();
	}
};

class SDLWindow {
public :
	SDLWindow(const std::string& title, int x, int y, int w, int h, Uint32 flags) throw(SDLError)
	: window(nullptr) {
		if ((window = ::SDL_CreateWindow(title.c_str(), x, y, w, h, flags)) == nullptr)
			throw SDLError();
	}
	virtual ~SDLWindow() {
		::SDL_DestroyWindow(window);
	}
	operator SDL_Window* () {
		return window;
	}
private :
	SDL_Window* window;
};

class SDLImage {
public :
	SDLImage(const std::string& file) throw(SDLError)
	: image(nullptr) {
		if ((image = ::IMG_Load(file.c_str())) == nullptr)
			throw SDLError();
	}
	virtual ~SDLImage() {
		::SDL_FreeSurface(image);
	}
	operator SDL_Surface* () {
		return image;
	}
	const int width() const throw(SDLError) {
		if (!image)
			throw SDLError("image do not loaded.");
		return image->w;
	}
	const int height() const throw(SDLError) {
		if (!image)
			throw SDLError("image do not loaded.");
		return image->h;
	}
	const SDL_PixelFormat* format() const throw(SDLError) {
		if (!image)
			throw SDLError("iamge do not loaded.");
		return image->format;
	}
private :
	SDL_Surface* image;
};
int main(int argc, char* argv[]) {
	static const Uint32 fps = 60;

	try {

		SDL sdl(SDL_INIT_EVERYTHING);

		SDLWindow window("First", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			600, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		SDLImage image("whale-watch.jpg");
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
	catch (const SDLError& err) {
		err.what();
	}

	return 0;
}