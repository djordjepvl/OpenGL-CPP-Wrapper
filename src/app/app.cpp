#include <app.h>

#include <input.h>

#include <camera.h>
#include <Object3D.h>
#include <skybox.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader *shader;
Shader *skyboxShader;
Skybox *skybox;
Camera *camera;
std::shared_ptr<Object3D> anvil, combi = nullptr;
float offsetX = 0.0f;

void App::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
    glClearColor(0.062f, 0.449f, 0.729f, 1.0f);

    shader = new Shader(SHADER_PATH + "default.vert", SHADER_PATH + "light.frag");
    shader->setVec3("sunDir", glm::normalize(glm::vec3(-0.3f, -0.2f, 0.0f))); 
    shader->setVec3("sunColor", {1.0, 1.0, 0.95});
    shader->setFloat("sunIntensity", 5.0f);
    shader->setVec3("ambientColor", {0.141, 0.467, 0.820});
    shader->setFloat("ambientIntensity", 2.0f);

    skyboxShader = new Shader(SHADER_PATH + "skybox.vert", SHADER_PATH + "skybox.frag");
    skybox = new Skybox("assets/cubemaps/above_the_sea");
    
    anvil = Object3D::LoadGLB(MODEL_PATH + "rifle__awp_weapon_model_cs2.glb", {-2, 0, 0}, 2);
    combi = Object3D::LoadGLB(MODEL_PATH + "combi.glb", {2,0,0}, 6, {0,-90,0});

    camera = new Camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    camera->setProjection(70.0f, 0.1f, 100.0f);
}

void App::update(float deltaTime, float currentTime) {
    if (Input::isKeyDown(GLFW_KEY_RIGHT)) offsetX += deltaTime*4;
    if (Input::isKeyDown(GLFW_KEY_LEFT)) offsetX -= deltaTime*4;
    offsetX *= 0.99f;

    camera->update(deltaTime);
    shader->setVec3("camPosition", camera->position);
    
    if (anvil) anvil->Rotate(0.0f, offsetX, 0.0f);
    if (combi) combi->Rotate(0.0f, offsetX, 0.0f);

    Input::mouseX = 0.0f;
    Input::mouseY = 0.0f;
    Input::deltaMouseX = 0.0f;
    Input::deltaMouseY = 0.0f;
}

void App::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader->Use();
    skybox->cubemap->assignToShader(shader, "skybox", 10);
    skybox->cubemap->Bind(10);
    
    if (anvil) anvil->Draw(shader, camera);
    if(combi) combi->Draw(shader, camera);

    skybox->Draw(skyboxShader, camera);
}

void App::close() {
    delete shader;
    delete camera;
    anvil = nullptr;
    combi = nullptr;
}
