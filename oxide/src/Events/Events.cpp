#include "Events.h"
#include "..//Window/Window.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include<GLFW/glfw3.h>
#include<string.h>

float oxide::Events::lastFrame = 0;
float oxide::Events::deltaTime = 0;

bool* oxide::Events::_keys;
uint* oxide::Events::_frames;
uint oxide::Events::_current = 0;
float oxide::Events::deltaX = 0.f;
float oxide::Events::deltaY = 0.f;
float oxide::Events::x = 0.f;
float oxide::Events::y = 0.f;
bool oxide::Events::_cursor_locked = false;
bool oxide::Events::_cursor_started = false;

#define _MOUSE_BUTTONS 1024

void cursor_position_callBack(GLFWwindow* window, double xpos, double ypos) {
    if (oxide::Events::_cursor_started) {
        oxide::Events::deltaX += xpos - oxide::Events::x;
        oxide::Events::deltaY += ypos - oxide::Events::y;
    }
    else {
        oxide::Events::_cursor_started = true;
    }
    oxide::Events::x = xpos;
    oxide::Events::y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
    if (action == GLFW_PRESS) {
        oxide::Events::_keys[_MOUSE_BUTTONS + button] = true;
        oxide::Events::_frames[_MOUSE_BUTTONS + button] = oxide::Events::_current;
    }
    else if (action == GLFW_RELEASE) {
        oxide::Events::_keys[_MOUSE_BUTTONS + button] = false;
        oxide::Events::_frames[_MOUSE_BUTTONS + button] = oxide::Events::_current;
    }
}

void key_callBack(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        oxide::Events::_keys[key] = true;
        oxide::Events::_frames[key] = oxide::Events::_current;
    }
    else if (action == GLFW_RELEASE) {
        oxide::Events::_keys[key] = false;
        oxide::Events::_frames[key] = oxide::Events::_current;
    }
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    oxide::Window::setWindowSize(width, height);
    glViewport(0, 0, width, height);
}

int oxide::Events::init() {
    GLFWwindow* window = Window::getWindow();
    _keys = new bool[1032];
    _frames = new uint[1032];

    memset(_keys, false, 1032);
    memset(_frames, 0, 1032);
    glfwSetKeyCallback(window, key_callBack);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callBack);
    glfwSetWindowSizeCallback(window, window_size_callback);
    return 0;
}

bool oxide::Events::pressed(int keycode) {
    if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
        return false;
    return _keys[keycode];
}

bool oxide::Events::jpressed(int keycode) {
    if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
        return false;
    return _keys[keycode] && _frames[keycode] == _current;
}

bool oxide::Events::clicked(int button) {
    int index = _MOUSE_BUTTONS + button;
    return _keys[index];
}

bool oxide::Events::jclicked(int button) {
    int index = _MOUSE_BUTTONS + button;
    return _keys[index] && _frames[index] == _current;
}

void oxide::Events::ToggleCursor(){
    _cursor_locked = !_cursor_locked;
    Window::setCursorMode(_cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
float oxide::Events::getTime(){
    return glfwGetTime();
}

void oxide::Events::pullEvents() {
    _current++;
    deltaX = 0.f;
    deltaY = 0.f;
    glfwPollEvents();

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}