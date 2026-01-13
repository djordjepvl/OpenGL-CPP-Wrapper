#include <app.h>

#include <input.h>

#include <camera.h>
#include <Object3D.h>
#include <skybox.h>
#include <shadowMap.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 sunDir = glm::normalize(glm::vec3(-0.3f, -0.3f, 0.0f));

Shader *shader;
Shader *shadowShader;
Shader *skyboxShader;
Skybox *skybox;
ShadowMap *shadowMap;
Camera *camera;
std::shared_ptr<Object3D> obj1, obj2, obj3 = nullptr;
float offsetX = 0.0f;

void App::init() {
    glEnable(GL_DEPTH_TEST);
    
    Texture::initDefaultTextures();


    shader = new Shader(SHADER_PATH + "light.vert", SHADER_PATH + "light.frag");
    shader->setVec3("sunDir", sunDir); 
    shader->setVec3("sunColor", {1.0, 1.0, 0.95});
    shader->setFloat("sunIntensity", 3.2f);
    shader->setVec3("ambientColor", {0.341, 0.567, 0.820});
    shader->setFloat("ambientIntensity", 4.0f);

    skyboxShader = new Shader(SHADER_PATH + "skybox.vert", SHADER_PATH + "skybox.frag");
    skybox = new Skybox("assets/cubemaps/above_the_sea");

    shadowShader = new Shader(SHADER_PATH + "shadow.vert", SHADER_PATH + "shadow.frag");
    shadowMap = new ShadowMap();
    shadowMap->init(2048*4);
    
    obj1 = Object3D::LoadGLB(MODEL_PATH + "animal__food__chicken_model_cs2.glb", {-2, -2, 0}, 2);
    obj2 = Object3D::LoadGLB(MODEL_PATH + "combi.glb", {2,0,0}, 6, {0,-90,0});
    obj3 = Object3D::LoadGLB(MODEL_PATH + "plane3.glb", {0,-2.5,0}, 200);

    camera = new Camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    camera->setProjection(70.0f, 0.1f, 100.0f);
}

void App::update(float deltaTime, float currentTime) {
    if (Input::isKeyDown(GLFW_KEY_RIGHT)) offsetX += deltaTime*4;
    if (Input::isKeyDown(GLFW_KEY_LEFT)) offsetX -= deltaTime*4;
    offsetX *= 0.99f;

    camera->update(deltaTime);
    shadowMap->update(sunDir, camera);
    
    if (obj1) obj1->Rotate(0.0f, offsetX, 0.0f);
    if (obj2) obj2->Rotate(0.0f, offsetX, 0.0f);
    
    Input::mouseX = 0.0f;
    Input::mouseY = 0.0f;
    Input::deltaMouseX = 0.0f;
    Input::deltaMouseY = 0.0f;
}

void App::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    shadowShader->Use();
    shadowShader->setMat4("shadowMatrix", shadowMap->matrix);
    
    shadowMap->Bind();
    if (obj1) obj1->Draw(shadowShader);
    if(obj2) obj2->Draw(shadowShader);
    if(obj3) obj3->Draw(shadowShader);
    shadowMap->Unbind();
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader->setVec3("camPosition", camera->position);
    shader->setMat4("camMatrix", camera->matrix);
    shader->setMat4("shadowMatrix", shadowMap->matrix);
    
    skybox->cubemap->assignToShader(shader, "skybox", 10);
    skybox->cubemap->Bind(10);

    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, shadowMap->depthMap);
    shader->setInt("shadowMap", 11);
    
    if (obj1) obj1->Draw(shader);
    if(obj2) obj2->Draw(shader);
    if(obj3) obj3->Draw(shader);

    skyboxShader->setMat4("camMatrix", camera->matrix);
    skybox->Draw(skyboxShader, camera);
}

void App::close() {
    delete shader;
    delete camera;
    obj1 = nullptr;
    obj2 = nullptr;
}
