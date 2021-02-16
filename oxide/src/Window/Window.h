#ifndef _OXIDE_WINDOW_H
#define _OXIDE_WINDOW_H

struct GLFWwindow;

namespace oxide {
	class Window {

		static GLFWwindow* window;

		static int width;
		static int height;

	public:
		
		static bool initialize(const char* title, const int width, const int height);

		static bool isShouldClose();

		static void setWindowSize(const int width, const int height);

		static const int get_WindowWidth();
		static const int get_WindowHeight();

		static void setCursorMode(int mode);

		static GLFWwindow* getWindow();

		static void setShouldClose(int value);
		static void swapBuffers();

		static void destroy();

	};
}

#endif 