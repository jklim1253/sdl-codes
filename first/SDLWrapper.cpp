#include "SDLWrapper.h"
namespace SDL {

////////////////////////////////////////////////////////////
// Error class implementation
////////////////////////////////////////////////////////////
Error::Error() : exception(), msg(::SDL_GetError()) {}
Error::Error(const std::string& m) : exception(), msg(m) {}
Error::~Error() throw() {}
const char* Error::what() const throw() {
	return msg.c_str();
}

////////////////////////////////////////////////////////////
// Library class implementation
////////////////////////////////////////////////////////////
Library::Library(Uint32 flags = 0) throw(Error) {
	if (::SDL_Init(flags) != 0)
		throw Error();
}
Library::~Library() {
	::SDL_Quit();
}

////////////////////////////////////////////////////////////
// Window class implementation
////////////////////////////////////////////////////////////
Window::Window(const std::string& title, int x, int y, int w, int h, Uint32 flags) throw(Error)
: window(nullptr) {
	if ((window = ::SDL_CreateWindow(title.c_str(), x, y, w, h, flags)) == nullptr)
		throw Error();
}
Window::~Window() {
	::SDL_DestroyWindow(window);
}
Window::operator SDL_Window* () {
	return window;
}

////////////////////////////////////////////////////////////
// Image class implementation
////////////////////////////////////////////////////////////
Image::Image(const std::string& file) throw(Error)
: image(nullptr) {
	if ((image = ::IMG_Load(file.c_str())) == nullptr)
		throw Error();
}
Image::~Image() {
	::SDL_FreeSurface(image);
}
Image::operator SDL_Surface* () {
	return image;
}
const int Image::width() const throw(Error) {
	if (!image)
		throw Error("image do not loaded.");
	return image->w;
}
const int Image::height() const throw(Error) {
	if (!image)
		throw Error("image do not loaded.");
	return image->h;
}
const SDL_PixelFormat* Image::format() const throw(Error) {
	if (!image)
		throw Error("iamge do not loaded.");
	return image->format;
}

} // namespace SDL