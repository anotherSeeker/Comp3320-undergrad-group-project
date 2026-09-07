#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

#include <string>

#include "../renderer/mesh.hpp"
#include "../renderer/shader.hpp"

#include "handles.h"

class AssetManager{
    std::unordered_map<int,std::shared_ptr<Mesh>> meshCache;
    std::unordered_map<int,std::shared_ptr<Shader>> shaderCache;

    public:

    SK_ASSET loadShader(const char* vertexFilePath,const char* fragmentFilePath);
    SK_ASSET loadObj(const char* FilePath);

    std::shared_ptr<Shader> getShader(SK_ASSET handle);
    std::shared_ptr<Mesh> getMesh(SK_ASSET handle);

    void unloadAll();
};