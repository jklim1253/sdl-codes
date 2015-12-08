#pragma once

#pragma warning(disable:4290)

#ifdef SDLWRAPPER_EXPORTS
#define SDLAPI __declspec(dllexport)
#else
#define SDLAPI __declspec(dllimport)
#endif // SDLWRAPPER_EXPORTS

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_image.h>
#include <exception>
#include <string>

class SDLAPI std::exception;

namespace SDL {

class ErrorImpl;
class SDLAPI Error : public std::exception {
public :
	Error();
	Error(const std::string& m);
	virtual ~Error() throw();
	virtual const char* what() const throw();
private :
	ErrorImpl* impl;
};

class SDLAPI Library {
public :
	Library(Uint32 flags /*=0*/) throw(Error);
	virtual ~Library();
};

class SDLAPI Window {
public :
	Window(const char* title, int x, int y, int w, int h, Uint32 flags) throw(Error);
	virtual ~Window();
	operator SDL_Window* ();

	void fullscreen(Uint32 flag) const;
private :
	SDL_Window* window;
};

class SDLAPI Image {
public :
	Image(const char* file) throw(Error);
	virtual ~Image();
	operator SDL_Surface* ();
	const int width() const throw(Error);
	const int height() const throw(Error);
	const SDL_PixelFormat* format() const throw(Error);
private :
	SDL_Surface* image;
};


} // namespace SDL