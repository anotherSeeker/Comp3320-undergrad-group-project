#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

#include <string>

#include "../renderer/mesh.hpp"
#include "../renderer/shader.hpp"

struct AssetHandle{
    int ID;
};

class AssetManager{
    std::unordered_map<int,std::shared_ptr<Mesh>> meshCache;
    std::unordered_map<int,std::shared_ptr<Shader>> shaderCache;

    public:

    AssetHandle loadShader(const char* vertexFilePath,const char* fragmentFilePath);
    AssetHandle loadObj(const char* FilePath);

    std::shared_ptr<Shader> getShader(AssetHandle handle);
    std::shared_ptr<Mesh> getMesh(AssetHandle handle);

    void unloadAll();

};