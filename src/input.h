#pragma once

#include <config.h>

class Input {
    public:
        static bool keys[1024];
        static bool mouseButtons[8];
        static double mouseX, mouseY;

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        
        static bool isKeyDown(int key);
        static bool isMouseButtonDown(int button);
};