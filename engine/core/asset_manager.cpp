#include "asset_manager.hpp"
#include "debugger.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

#include <fstream>
#include <sstream>
#include <string>
#include <format>
#include <cstring>

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
