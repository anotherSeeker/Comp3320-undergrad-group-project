#pragma once

#include <unordered_map>
#include <memory>

#include <string>

#include "../renderer/mesh.hpp"
#include "../renderer/shader.hpp"

class AssetManager{
    std::unordered_map<std::string,std::weak_ptr<Mesh>> meshCache;
    std::unordered_map<std::string,std::weak_ptr<Shader>> shaderCache;

    public:

    std::shared_ptr<Shader> loadShader(const char* vertexFilePath,const char* fragmentFilePath);
    std::shared_ptr<Mesh> loadObj(const char* FilePath);
};