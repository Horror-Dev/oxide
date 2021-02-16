#include "Window.h"
#include "..//oxide/oxide.h"

#include <GLFW/glfw3.h>

int			oxide::Window::width;
int			oxide::Window::height;
GLFWwindow* oxide::Window::window;

void GLFWErrorCallback(int code, const char* content) {
	oxide::Engine::Logger* logger = oxide::Engine::getLastInstance()->getLogger();

	*logger << "GLFW error: {" << code << "} " << "\n" << content << "\n";
	logger->add_log(oxide::Engine::Logger::log_type::lt_error);
}

bool oxide::Window::isShouldClose(){
	return glfwWindowShouldClose(window);
}

void oxide::Window::setWindowSize(const int width, const int height){
	oxide::Window::width = width;
	oxide::Window::height = height;
}

void oxide::Window::setCursorMode(int mode){
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void oxide::Window::setShouldClose(int value){
	glfwSetWindowShouldClose(window, value);
}

bool oxide::Window::pre_initialize(){
	oxide::Engine::Logger* logger = oxide::Engine::getLastInstance()->getLogger();

	if (!glfwInit()) {
		*logger << "Cannot initalize GLFW!" << "\n";
		logger->add_log(oxide::Engine::Logger::log_type::lt_error);
		return false;
	}

	return true;
}

bool oxide::Window::initialize(const char* title, const int width, const int height){
	oxide::Window::width = width;
	oxide::Window::height = height;
	
	glfwSetErrorCallback(GLFWErrorCallback);

	oxide::Engine::Logger* logger = oxide::Engine::getLastInstance()->getLogger();
	

	window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window) {
		*logger << "Cannot create  window!" << "\n";
		logger->add_log(oxide::Engine::Logger::log_type::lt_error);
		return false;
	}

	glfwMakeContextCurrent(window);

	return true;
}

const int oxide::Window::get_WindowWidth(){
	return oxide::Window::width;
}

const int oxide::Window::get_WindowHeight(){
	return oxide::Window::height;
}

GLFWwindow* oxide::Window::getWindow(){
	return window;
}

void oxide::Window::swapBuffers(){
	glfwSwapBuffers(window);
}

void oxide::Window::destroy(){
	glfwDestroyWindow(window);
	glfwTerminate();
}
