#include <skybox.h>


Skybox::Skybox(std::string folderPath){
    cubemap = std::make_shared<Texture>(folderPath, DIRECTORY, 10);
    setupMesh();
}

void Skybox::Draw(Shader *skyboxShader, Camera *camera) {
    glDepthFunc(GL_LEQUAL); 

    skyboxShader->Use();

    skyboxShader->setMat4("camMatrix", camera->projection * glm::mat4(glm::mat3(camera->view)));

    glBindVertexArray(VAO);
    cubemap->Bind(10);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

void Skybox::setupMesh() {
    float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,    -1.0f, -1.0f, -1.0f,     1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,     1.0f,  1.0f, -1.0f,    -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,    -1.0f, -1.0f, -1.0f,    -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,    -1.0f,  1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,     1.0f, -1.0f,  1.0f,     1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,     1.0f,  1.0f, -1.0f,     1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,    -1.0f,  1.0f,  1.0f,     1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,     1.0f, -1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,     1.0f,  1.0f, -1.0f,     1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,    -1.0f,  1.0f,  1.0f,    -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,     1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,     1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}