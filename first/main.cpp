#include "../SDLWrapper/SDLWrapper.h"
using namespace SDL;

namespace SDL {
struct Rect : public SDL_Rect {

};
struct Color {
	Uint8 r, g, b;
	Color() :r(0), g(0), b(0) {}
	Color(Uint8 _r, Uint8 _g, Uint8 _b) : r(_r), g(_g), b(_b) {}
	Color(Uint32 rgb) : r((rgb>>16) & 0xFF), g((rgb>>8) & 0xFF), b(rgb & 0xFF) {}
	Uint32 use(SDL_PixelFormat* _pixelformat) const {
		pixelformat = _pixelformat;
		return ::SDL_MapRGB(pixelformat, r, g, b);
	}
	operator Uint32 () const {
		return ::SDL_MapRGB(pixelformat, r, g, b);
	}
private :
	mutable SDL_PixelFormat* pixelformat;
};
class BasicDraw {
	typedef SDL_Surface SurfaceType;
	typedef SurfaceType* SurfacePointer;
public :
	BasicDraw(const SurfacePointer& s) : surface(s) {}
	virtual ~BasicDraw() {}

	void FillRect(const Rect* r, const Color& clr) throw(Error) {
		if (::SDL_FillRect(surface, r, clr.use(surface->format)) != 0)
			throw Error();
	}
private :
	SurfacePointer surface;
};
}

class Application {
public :
	Application() throw(Error) {
		library = new Library(SDL_INIT_EVERYTHING);

		window = new Window("First", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			600, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	}
	~Application() {
		if (window)
			delete window;

		if (library)
			delete library;
	}

	void Initialize() {

	}
	void MainLoop() {
		const Uint32 fps = 30;

		Image image("whale-watch.jpg");
		SDL_Rect src = { 0,0,image.width(), image.height() };
		SDL_Rect dst = { 0,0,image.width(), image.height() };

		SDL_Surface* screen = ::SDL_GetWindowSurface(*window);
		if (screen == nullptr) {
			return;
		}

		int& x = dst.x;
		Uint32 previous = ::SDL_GetTicks() + 1000/fps;
		SDL_Event event;
		bool quit = false;
		bool fullscreen = false;
		while (!quit) {
			while (::SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) quit = true;
				if (event.type == SDL_KEYDOWN) {
					if (fullscreen)
						window->fullscreen(0);
					else
						window->fullscreen(SDL_WINDOW_FULLSCREEN_DESKTOP);

					fullscreen = !fullscreen;
				}
			}

			while (!SDL_TICKS_PASSED(::SDL_GetTicks(), previous)) {
				::SDL_Delay(previous - ::SDL_GetTicks());
			}
			previous = ::SDL_GetTicks() + 1000 / fps;

			// update screen contents.
			++x;
			if (x > 100)
				x = 0;

			// draw screen.
			screen = ::SDL_GetWindowSurface(*window);

			::SDL_FillRect(screen, nullptr, ::SDL_MapRGB(screen->format, 0x99, 0x66, 0xcc));

			::SDL_BlitSurface(image, &src, screen, &dst);

			::SDL_UpdateWindowSurface(*window);
		}
	}
private :
	Library* library;
	Window* window;
};
int main(int argc, char* argv[]) {

	try {
		Application app;

		app.Initialize();

		app.MainLoop();
	}
	catch (const Error& err) {
		err.what();
	}

	return 0;
}