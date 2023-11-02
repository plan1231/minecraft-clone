//
// Created by Phil Lan on 2023-10-28.
//

#ifndef MINECRAFT_CLONE_SHADER_H
#define MINECRAFT_CLONE_SHADER_H
#include <unordered_map>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
    unsigned int program = 0;
    std::unordered_map<std::string, unsigned int> uniforms;

    static std::string getSource(const std::string& filename);
    static unsigned int compileShader(int shaderType, const std::string& filename);

    unsigned int getLocation(const std::string& name);
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void use() const;

    void setInt(const std::string& name, int value);
    void setMatrix4(const std::string& name, const glm::mat4& mat);
};

using ShaderPtr = std::shared_ptr<Shader>;
#endif //MINECRAFT_CLONE_SHADER_H
