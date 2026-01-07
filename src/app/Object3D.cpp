#include <Object3D.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>



Object3D::Object3D(std::vector<std::shared_ptr<Mesh>> &meshes){
    this->meshes = meshes;
}

void Object3D::Draw(Shader *shader, Camera *camera) {
    shader->Use();

    shader->setMat4("model", getModelMatrix());
    shader->setMat4("camMatrix", camera->getCamMatrix());
    
    for (std::shared_ptr<Mesh> mesh : meshes) mesh->Draw(shader);
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

    std::map<int, std::shared_ptr<Texture>> loadedTextures;
    std::vector<std::shared_ptr<Mesh>> loadedMeshes;

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
                
                float* pos = (float*)(posBase + i * posStride);
                v.position = glm::vec3(pos[0], pos[1], pos[2]);
                
                float* norm = (float*)(normBase + i * normStride);
                v.normal = glm::vec3(norm[0], norm[1], norm[2]);
                
                float* uv = (float*)(uvBase + i * uvStride);
                v.UV = glm::vec2(uv[0], uv[1]);
                
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

            std::shared_ptr<Material> meshMaterial = nullptr;
            if (primitive.material >= 0) {
                const auto& gltfMat = model.materials[primitive.material];
                int texIdx = gltfMat.pbrMetallicRoughness.baseColorTexture.index;

                if (texIdx >= 0) {
                    int imgIdx = model.textures[texIdx].source;
                    
                    // Check cache first so we don't re-upload to GPU
                    if (loadedTextures.find(imgIdx) == loadedTextures.end()) {
                        const auto& img = model.images[imgIdx];
                        loadedTextures[imgIdx] = std::make_shared<Texture>(
                            img.image.data(), img.width, img.height, img.component, DIFFUSE, 0
                        );
                    }
                    meshMaterial = std::make_shared<Material>(loadedTextures[imgIdx]);
                }
            }

            loadedMeshes.push_back(std::make_shared<Mesh>(vertices, indices, meshMaterial));
        }
    }
    // Still returning nullptr for texture until you're ready to add the glbTexture logic
    return std::make_shared<Object3D>(loadedMeshes);
}