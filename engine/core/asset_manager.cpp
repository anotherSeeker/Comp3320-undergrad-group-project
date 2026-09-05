#include "asset_manager.hpp"
#include "debugger.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <format>
#include <cstring>

// found this on stack overflow https://stackoverflow.com/a/57595105
template <typename T, typename... Rest>
void hash_combine(std::size_t& seed, const T& v, const Rest&... rest)
{
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hash_combine(seed, rest), ...);
}

// found this from a yt video https://www.youtube.com/watch?v=jdiPVfIHmEA
namespace std{
    template<> struct hash<Vertex>{
        size_t operator()(Vertex const &vertex) const {
            size_t seed = 0;
            hash_combine(seed,vertex.position,vertex.normal,vertex.UV);

            return seed;
        }
    };
}


std::string loadFile(std::string filePath){
    std::ifstream file;
    file.open(filePath);

    if(!file.is_open()){
        Debugger::print(std::format("cannot loadfile {}",filePath));
        return "";
    }
    
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}

std::shared_ptr<Shader> AssetManager::loadShader(const char* vertexFilePath,const char* fragmentFilePath){
    std::string assetID = std::string(vertexFilePath).append(":").append(fragmentFilePath);

    if(shaderCache.contains(assetID)){
        std::weak_ptr<Shader> &shaderptr = shaderCache.at(assetID);

        if(!shaderptr.expired()){
            return shaderptr.lock();
        }

        shaderptr.reset();
        shaderCache.erase(assetID);
    }

    std::string vertexSource = loadFile(vertexFilePath);
    std::string fragmentSource = loadFile(fragmentFilePath);

    const char* vertexRaw = vertexSource.c_str();
    const char* fragmentRaw = fragmentSource.c_str();

    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    assert(shader->init(vertexSource,fragmentSource) == true); // replace this with default shader control logic

    std::weak_ptr<Shader> shader_ptr = shader;

    shaderCache.insert_or_assign(assetID,shader_ptr);

    return shader_ptr.lock();
}

std::shared_ptr<Mesh> AssetManager::loadObj(const char* FilePath){
    std::string assetID = std::string(FilePath);

    if(meshCache.contains(assetID)){
        std::weak_ptr<Mesh> &meshptr = meshCache.at(assetID);

        if(!meshptr.expired()){
            return meshptr.lock();
        }

        meshptr.reset();
        meshCache.erase(assetID);
    }

    tinyobj::attrib_t attribute;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn,err;
    
    if(tinyobj::LoadObj(&attribute,&shapes,&materials,&warn,&err,FilePath) == false){
        Debugger::print(warn + err);
        return nullptr;
    };

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    std::unordered_map<Vertex,uint32_t> uniqueVertices;

    for(const auto &shape : shapes){
        for(const auto &index : shape.mesh.indices){
            Vertex vertex{};
            if(index.vertex_index >= 0){
                vertex.position = {
                    attribute.vertices[3 * index.vertex_index + 0],
                    attribute.vertices[3 * index.vertex_index + 1],
                    attribute.vertices[3 * index.vertex_index + 2],
                };
            }

            if(index.normal_index >= 0){
                vertex.normal = {
                    attribute.normals[3 * index.normal_index + 0],
                    attribute.normals[3 * index.normal_index + 1],
                    attribute.normals[3 * index.normal_index + 2],
                };
            }

            if(index.texcoord_index >= 0){
                vertex.UV = {
                    attribute.texcoords[2 * index.texcoord_index + 0],
                    attribute.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if(uniqueVertices.count(vertex) == 0){
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }


    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertices,indices);

    std::weak_ptr<Mesh> mesh_ptr = mesh;

    meshCache.insert_or_assign(assetID,mesh_ptr);

    return mesh_ptr.lock();
}
