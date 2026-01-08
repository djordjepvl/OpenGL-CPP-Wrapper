#include <app.h>

#include <input.h>

#include <camera.h>
#include <Object3D.h>
#include <skybox.h>
#include <shadowMap.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 sunDir = glm::normalize(glm::vec3(-0.3f, -0.2f, 0.0f));

Shader *shader;
Shader *shadowShader;
Shader *skyboxShader;
Skybox *skybox;
ShadowMap *shadowMap;
Camera *camera;
std::shared_ptr<Object3D> anvil, combi, plane = nullptr;
float offsetX = 0.0f;

void App::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    shader = new Shader(SHADER_PATH + "light.vert", SHADER_PATH + "light.frag");
    shader->setVec3("sunDir", sunDir); 
    shader->setVec3("sunColor", {1.0, 1.0, 0.95});
    shader->setFloat("sunIntensity", 3.2f);
    shader->setVec3("ambientColor", {0.141, 0.467, 0.820});
    shader->setFloat("ambientIntensity", 6.0f);

    skyboxShader = new Shader(SHADER_PATH + "skybox.vert", SHADER_PATH + "skybox.frag");
    skybox = new Skybox("assets/cubemaps/above_the_sea");

    shadowShader = new Shader(SHADER_PATH + "shadow.vert", SHADER_PATH + "shadow.frag");
    shadowMap = new ShadowMap();
    shadowMap->init(2048);
    
    anvil = Object3D::LoadGLB(MODEL_PATH + "rifle__awp_weapon_model_cs2.glb", {-2, 0, 0}, 2);
    combi = Object3D::LoadGLB(MODEL_PATH + "combi.glb", {2,0,0}, 6, {0,-90,0});
    plane = Object3D::LoadGLB(MODEL_PATH + "plane.glb", {0,-2.5,0}, 20);

    camera = new Camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    camera->setProjection(70.0f, 0.1f, 100.0f);
}

void App::update(float deltaTime, float currentTime) {
    if (Input::isKeyDown(GLFW_KEY_RIGHT)) offsetX += deltaTime*4;
    if (Input::isKeyDown(GLFW_KEY_LEFT)) offsetX -= deltaTime*4;
    offsetX *= 0.99f;

    camera->update(deltaTime);
    shadowMap->update(sunDir, camera);
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
    shadowShader->Use();
    shadowShader->setMat4("shadowMatrix", shadowMap->matrix);

    shadowMap->bind();
    if (anvil) anvil->Draw(shadowShader);
    if(combi) combi->Draw(shadowShader);
    if(plane) plane->Draw(shadowShader);
    shadowMap->unbind();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->setMat4("camMatrix", camera->matrix);
    shader->setMat4("shadowMatrix", shadowMap->matrix);
    
    skybox->cubemap->assignToShader(shader, "skybox", 10);
    skybox->cubemap->Bind(10);

    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, shadowMap->depthMap);
    shader->setInt("shadowMap", 11);
    
    if (anvil) anvil->Draw(shader);
    if(combi) combi->Draw(shader);
    if(plane) plane->Draw(shader);

    skyboxShader->setMat4("camMatrix", camera->matrix);
    skybox->Draw(skyboxShader, camera);
}

void App::close() {
    delete shader;
    delete camera;
    anvil = nullptr;
    combi = nullptr;
}
