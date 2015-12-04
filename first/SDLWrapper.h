#pragma once

#pragma warning(disable:4290)

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_image.h>
#include <exception>
#include <string>

namespace SDL {

class Error : public std::exception {
public :
	Error();
	Error(const std::string& m);
	virtual ~Error() throw();
	virtual const char* what() const throw();
private :
	std::string msg;
};

class Library {
public :
	Library(Uint32 flags /*=0*/) throw(Error);
	virtual ~Library();
};

class Window {
public :
	Window(const std::string& title, int x, int y, int w, int h, Uint32 flags) throw(Error);
	virtual ~Window();
	operator SDL_Window* ();
private :
	SDL_Window* window;
};

class Image {
public :
	Image(const std::string& file) throw(Error);
	virtual ~Image();
	operator SDL_Surface* ();
	const int width() const throw(Error);
	const int height() const throw(Error);
	const SDL_PixelFormat* format() const throw(Error);
private :
	SDL_Surface* image;
};

} // namespace SDL