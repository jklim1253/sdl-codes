#include "SDLWrapper.h"
namespace SDL {

////////////////////////////////////////////////////////////
// Error class implementation
////////////////////////////////////////////////////////////
class ErrorImpl {
public :
	ErrorImpl(const std::string& m) : msg(m) {}
	~ErrorImpl() {}
	operator const char* () {
		return msg.c_str();
	}
private :
	std::string msg;
};
Error::Error() 
: exception(), impl(nullptr) {
	impl = new ErrorImpl(::SDL_GetError());
}
Error::Error(const std::string& m)
: exception(), impl(nullptr) {
	impl = new ErrorImpl(m);
}
Error::~Error() throw() {
	if (impl)
		delete impl;
}
const char* Error::what() const throw() {
	if (impl)
		return *impl;
	return nullptr;
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
Window::Window(const char* title, int x, int y, int w, int h, Uint32 flags) throw(Error)
: window(nullptr) {
	if ((window = ::SDL_CreateWindow(title, x, y, w, h, flags)) == nullptr)
		throw Error();
}
Window::~Window() {
	::SDL_DestroyWindow(window);
}
Window::operator SDL_Window* () {
	return window;
}
void Window::fullscreen(Uint32 flag) const throw(Error) {
	if (!::SDL_SetWindowFullscreen(window, flag))
		throw Error();
}

////////////////////////////////////////////////////////////
// Image class implementation
////////////////////////////////////////////////////////////
Image::Image(const char* file) throw(Error)
: image(nullptr) {
	if ((image = ::IMG_Load(file)) == nullptr)
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