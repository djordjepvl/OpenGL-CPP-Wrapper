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

    if (!loader.LoadBinaryFromFile(&model, &err, &warn, path)) return nullptr;

    std::vector<std::shared_ptr<Mesh>> meshes;

    for (const auto& gltfMesh : model.meshes) {
        for (const auto& primitive : gltfMesh.primitives) {
            
            // 1. Get Accessors
            const auto& posAcc = model.accessors[primitive.attributes.at("POSITION")];
            const auto& normAcc = model.accessors[primitive.attributes.at("NORMAL")];
            const auto& uvAcc = model.accessors[primitive.attributes.at("TEXCOORD_0")];
            const auto& idxAcc = model.accessors[primitive.indices];

            // 2. Helper with Stride support
            auto getBufferInfo = [&](const tinygltf::Accessor& acc) {
                const auto& view = model.bufferViews[acc.bufferView];
                return std::make_pair(
                    &model.buffers[view.buffer].data[view.byteOffset + acc.byteOffset],
                    acc.ByteStride(view) // Critical: skip the correct number of bytes
                );
            };

            auto [posBase, posStride] = getBufferInfo(posAcc);
            auto [normBase, normStride] = getBufferInfo(normAcc);
            auto [uvBase, uvStride] = getBufferInfo(uvAcc);
            auto [idxBase, idxStride] = getBufferInfo(idxAcc);

            // 3. Assemble Vertices
            std::vector<Vertex> vertices;
            for (size_t i = 0; i < posAcc.count; i++) {
                Vertex v;
                // Correctly jump memory using the stride
                const float* p = (const float*)(posBase + (i * posStride));
                const float* n = (const float*)(normBase + (i * normStride));
                const float* u = (const float*)(uvBase + (i * uvStride));

                v.position = glm::vec3(p[0], p[1], p[2]);
                v.normal   = glm::vec3(n[0], n[1], n[2]);
                // Flip UV Y-axis for OpenGL
                v.UV       = glm::vec2(u[0], 1.0f - u[1]); 
                v.color    = glm::vec3(1.0f); 
                vertices.push_back(v);
            }

            // 4. Handle 16-bit vs 32-bit Indices
            std::vector<unsigned int> indices;
            for (size_t i = 0; i < idxAcc.count; i++) {
                if (idxAcc.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                    indices.push_back(((uint16_t*)idxBase)[i]);
                } else {
                    indices.push_back(((uint32_t*)idxBase)[i]);
                }
            }

            meshes.push_back(std::make_shared<Mesh>(vertices, indices));
        }
    }
    // Still returning nullptr for texture until you're ready to add the glbTexture logic
    return std::make_shared<Object3D>(meshes, nullptr);
}