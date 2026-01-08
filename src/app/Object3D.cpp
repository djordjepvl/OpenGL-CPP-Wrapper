#include <Object3D.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>


Object3D::Object3D(std::vector<std::shared_ptr<Mesh>> &meshes){
    this->meshes = meshes;
}

void Object3D::Draw(Shader *shader) {
    shader->Use();

    shader->setMat4("model", getModelMatrix());
    
    for (std::shared_ptr<Mesh> mesh : meshes) mesh->Draw(shader);
}
glm::mat4 Object3D::getModelMatrix(){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model *= glm::mat4_cast(rotation);
    model = glm::scale(model, scale);
    return model;
}


std::shared_ptr<Object3D> Object3D::LoadGLB(const std::string& path, glm::vec3 pos, float s, glm::vec3 rot) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    if (!loader.LoadBinaryFromFile(&model, &err, &warn, path)) {
        std::cout << "Object3D: Could not find model \"" << path << "\"" << std::endl;
        return nullptr;
    }

    std::map<int, std::shared_ptr<Texture>> loadedTextures;
    std::vector<std::shared_ptr<Mesh>> loadedMeshes;

    // 1. Iterate through NODES instead of meshes to get transform data
    for (const auto& node : model.nodes) {
        if (node.mesh < 0) continue; // Skip nodes without geometry

        // 2. Extract Node Matrix (Handling T/R/S or Matrix)
        glm::mat4 nodeMatrix = glm::mat4(1.0f);
        if (node.matrix.size() == 16) {
            nodeMatrix = glm::make_mat4(node.matrix.data());
        } else {
            // 1. Handle Translation
            if (node.translation.size() == 3) {
                nodeMatrix = glm::translate(nodeMatrix, glm::vec3(
                    (float)node.translation[0], 
                    (float)node.translation[1], 
                    (float)node.translation[2]
                ));
            }
            // 2. Handle Rotation (Quaternion)
            if (node.rotation.size() == 4) {
                glm::quat q = glm::quat(
                    (float)node.rotation[3], // GLM quat is (w, x, y, z)
                    (float)node.rotation[0], 
                    (float)node.rotation[1], 
                    (float)node.rotation[2]
                );
                nodeMatrix *= glm::mat4_cast(q);
            }
            // 3. Handle Scale
            if (node.scale.size() == 3) {
                nodeMatrix = glm::scale(nodeMatrix, glm::vec3(
                    (float)node.scale[0], 
                    (float)node.scale[1], 
                    (float)node.scale[2]
                ));
            }
        }

        // Pre-calculate Normal Matrix for the bake
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(nodeMatrix)));

        const auto& gltfMesh = model.meshes[node.mesh];
        for (const auto& primitive : gltfMesh.primitives) {
            
            // --- ACCESSOR & BUFFER INFO (Same as your original) ---
            const auto& posAcc = model.accessors[primitive.attributes.at("POSITION")];
            const auto& normAcc = model.accessors[primitive.attributes.at("NORMAL")];
            const auto& uvAcc = model.accessors[primitive.attributes.at("TEXCOORD_0")];
            const auto& idxAcc = model.accessors[primitive.indices];

            auto getBufferInfo = [&](const tinygltf::Accessor& acc) {
                const auto& view = model.bufferViews[acc.bufferView];
                return std::make_pair(&model.buffers[view.buffer].data[view.byteOffset + acc.byteOffset], acc.ByteStride(view));
            };

            auto [posBase, posStride] = getBufferInfo(posAcc);
            auto [normBase, normStride] = getBufferInfo(normAcc);
            auto [uvBase, uvStride] = getBufferInfo(uvAcc);
            auto [idxBase, idxStride] = getBufferInfo(idxAcc);

            // 3. Assemble Vertices with BAKING
            std::vector<Vertex> vertices;
            for (size_t i = 0; i < posAcc.count; i++) {
                Vertex v;
                float* p = (float*)(posBase + i * posStride);
                float* n = (float*)(normBase + i * normStride);
                float* u = (float*)(uvBase + i * uvStride);

                // Multiply local data by nodeMatrix to "fix" orientation/position
                v.position = glm::vec3(nodeMatrix * glm::vec4(p[0], p[1], p[2], 1.0f));
                v.normal   = glm::normalize(normalMatrix * glm::vec3(n[0], n[1], n[2]));
                v.UV       = glm::vec2(u[0], u[1]);
                
                vertices.push_back(v);
            }

            // 4. Handle Indices (Same as original)
            std::vector<unsigned int> indices;
            for (size_t i = 0; i < idxAcc.count; i++) {
                if (idxAcc.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
                    indices.push_back(((uint16_t*)idxBase)[i]);
                else
                    indices.push_back(((uint32_t*)idxBase)[i]);
            }

            // 5. Material & Texture Loading (Same as original)
            std::shared_ptr<Material> meshMaterial = nullptr;
            if (primitive.material >= 0) {
                const auto& gltfMat = model.materials[primitive.material];
                auto loadTex = [&](int texIdx, TextureType type) {
                    if (texIdx < 0) return;
                    int imgIdx = model.textures[texIdx].source;
                    if (loadedTextures.find(imgIdx) == loadedTextures.end()) {
                        const auto& img = model.images[imgIdx];
                        loadedTextures[imgIdx] = std::make_shared<Texture>(
                            img.image.data(), img.width, img.height, img.component, type, (type == DIFFUSE)
                        );
                    }
                };
                loadTex(gltfMat.pbrMetallicRoughness.baseColorTexture.index, DIFFUSE);
                loadTex(gltfMat.normalTexture.index, NORMAL);
                loadTex(gltfMat.pbrMetallicRoughness.metallicRoughnessTexture.index, SPECULAR);
                meshMaterial = std::make_shared<Material>(loadedTextures);
            }

            loadedMeshes.push_back(std::make_shared<Mesh>(vertices, indices, meshMaterial));
        }
    }

    auto obj = std::make_shared<Object3D>(loadedMeshes);
    obj->position = pos;
    obj->SetScale(s);
    obj->SetRotation(rot.x, rot.y, rot.z);
    return obj;
}