#include <Object3D.h>

Object3D::Object3D(std::vector<Mesh*> &meshes, Texture *atlasTexture){
    this->meshes = meshes;
    this->atlasTexture = atlasTexture;
}
Object3D::Object3D(std::string &stbFilePath, Texture *atlasTexture){
    //load all meshes from stb file into this new object3d
    //also load the diffuse atlas file that the meshes will share
}
Object3D::~Object3D(){
    
}

void Object3D::Draw(Shader *shader, Camera *camera) {
    shader->Use();

    shader->setMat4("model", getModelMatrix());
    shader->setMat4("camMatrix", camera->getCamMatrix());

    if (atlasTexture) {
        atlasTexture->assignToShader(shader, "diffuse0"); 
        atlasTexture->Bind();
    }
    
    for (Mesh* mesh : meshes) mesh->Draw();
}
glm::mat4 Object3D::getModelMatrix(){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model *= glm::mat4_cast(rotation);
    model = glm::scale(model, scale);
    return model;
}