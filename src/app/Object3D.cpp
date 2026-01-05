#include <Object3D.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>



Object3D::Object3D(std::vector<std::shared_ptr<Mesh>> &meshes, std::shared_ptr<Texture> atlasTexture){
    this->meshes = meshes;
    this->atlasTexture = atlasTexture;
}

void Object3D::Draw(Shader *shader, Camera *camera) {
    shader->Use();

    shader->setMat4("model", getModelMatrix());
    shader->setMat4("camMatrix", camera->getCamMatrix());

    if (atlasTexture) {
        atlasTexture->assignToShader(shader, "diffuse0"); 
        atlasTexture->Bind();
    }
    
    for (std::shared_ptr<Mesh> mesh : meshes) mesh->Draw();
}
glm::mat4 Object3D::getModelMatrix(){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model *= glm::mat4_cast(rotation);
    model = glm::scale(model, scale);
    return model;
}


std::shared_ptr<Object3D> Object3D::LoadGLB(const std::string& path) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    if (!loader.LoadBinaryFromFile(&model, &err, &warn, path)) {
        std::cout << "GLB Load Error: " << err << std::endl;
        std::cout << "GLB Warning: " << warn << std::endl;
        return nullptr; 
    }

    std::vector<std::shared_ptr<Mesh>> meshes;

    for (const auto& gltfMesh : model.meshes) {
        for (const auto& primitive : gltfMesh.primitives) {
            
            // 1. Get Accessors for attributes
            const tinygltf::Accessor& posAcc = model.accessors[primitive.attributes.at("POSITION")];
            const tinygltf::Accessor& normAcc = model.accessors[primitive.attributes.at("NORMAL")];
            const tinygltf::Accessor& uvAcc = model.accessors[primitive.attributes.at("TEXCOORD_0")];
            const tinygltf::Accessor& idxAcc = model.accessors[primitive.indices];

            // 2. Helper to get raw data pointers
            auto getData = [&](const tinygltf::Accessor& acc) {
                const auto& view = model.bufferViews[acc.bufferView];
                return &model.buffers[view.buffer].data[view.byteOffset + acc.byteOffset];
            };

            const float* posPtr = (const float*)getData(posAcc);
            const float* normPtr = (const float*)getData(normAcc);
            const float* uvPtr = (const float*)getData(uvAcc);
            const uint16_t* idxPtr = (const uint16_t*)getData(idxAcc);

            // 3. Assemble Vertices
            std::vector<Vertex> vertices;
            for (size_t i = 0; i < posAcc.count; i++) {
                Vertex v;
                v.position = glm::vec3(posPtr[i*3], posPtr[i*3+1], posPtr[i*3+2]);
                v.normal   = glm::vec3(normPtr[i*3], normPtr[i*3+1], normPtr[i*3+2]);
                v.UV = glm::vec2(uvPtr[i*2], uvPtr[i*2+1]);
                vertices.push_back(v);
            }

            // 4. Create Mesh
            std::vector<unsigned int> indices(idxPtr, idxPtr + idxAcc.count);
            meshes.push_back(std::make_shared<Mesh>(vertices, indices));
        }
    }
    return std::make_shared<Object3D>(meshes, nullptr);
}