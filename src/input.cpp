#include <input.h>

bool Input::keys[1024] = {0};
bool Input::mouseButtons[8] = {false};
double Input::mouseX = 0.0f;
double Input::mouseY = 0.0f;
double Input::deltaMouseX = 0.0f;
double Input::deltaMouseY = 0.0f;

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key < 0 || key >= 1024) return;
    keys[key] = (action != GLFW_RELEASE);
}
void Input::mouse_callback(GLFWwindow* window, double xpos, double ypos){
    static double lastX = xpos;
    static double lastY = ypos;

    deltaMouseX = xpos - lastX;
    deltaMouseY = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    mouseX = xpos;
    mouseY = ypos;
}
void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button < 0 || button >= 8) return;
    mouseButtons[button] = (action != GLFW_RELEASE);
}

bool Input::isKeyDown(int key){
    return (key < 0 || key >= 1024) ? false : keys[key];
}
bool Input::isMouseButtonDown(int button){
    return (button < 0 || button >= 8) ? false : mouseButtons[button];
}