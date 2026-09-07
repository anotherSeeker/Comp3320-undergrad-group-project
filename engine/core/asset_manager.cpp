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

#include <functional>

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

SK_ASSET AssetManager::loadShader(const char* vertexFilePath,const char* fragmentFilePath){
    int assetID = std::hash<std::string>{}(std::string(vertexFilePath).append(fragmentFilePath));

    std::string vertexSource = loadFile(vertexFilePath);
    std::string fragmentSource = loadFile(fragmentFilePath);

    const char* vertexRaw = vertexSource.c_str();
    const char* fragmentRaw = fragmentSource.c_str();

    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    assert(shader->init(vertexSource,fragmentSource) == true); // replace this with default shader control logic

    shaderCache.insert_or_assign(assetID,shader);

    return {assetID};
}

SK_ASSET AssetManager::loadObj(const char* FilePath){
    int assetID = std::hash<std::string>{}(FilePath);

    tinyobj::attrib_t attribute;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn,err;
    
    bool result = tinyobj::LoadObj(&attribute,&shapes,&materials,&warn,&err,FilePath);
    
    if(!result){
        Debugger::print(warn + err);
    };

    assert(result == true);
    
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

    meshCache.insert_or_assign(assetID,mesh);

    return {assetID};
}


std::shared_ptr<Shader> AssetManager::getShader(SK_ASSET handle){
    assert(shaderCache.contains(handle.ID));

    return shaderCache.at(handle.ID);
}

std::shared_ptr<Mesh> AssetManager::getMesh(SK_ASSET handle){
    assert(meshCache.contains(handle.ID));

    return meshCache.at(handle.ID);
}

void AssetManager::unloadAll(){
    meshCache.clear();
    shaderCache.clear();
}