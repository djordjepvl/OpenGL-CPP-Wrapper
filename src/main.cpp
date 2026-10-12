#include <app.h>

#include <input.h>

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    App::init();

    glfwSetKeyCallback(window, Input::key_callback);
    glfwSetCursorPosCallback(window, Input::mouse_callback);
    glfwSetMouseButtonCallback(window, Input::mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float deltaTime = 0.0f;
    float prevTime = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;
        
        App::update(deltaTime, currentTime);
        App::render();

        glfwSwapBuffers(window);
    }
    App::close();
    glfwTerminate();
    return 0;
}