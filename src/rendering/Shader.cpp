//
// Created by Phil Lan on 2023-10-28.
//

#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    program = glCreateProgram();

    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexPath);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);

        std::cout << "cannot link shader program" << std::endl;
        std::cout << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

std::string Shader::getSource(const std::string &filename) {
    std::ifstream file;
    /*
     * badbit: failure to open file
     * failbit: formmated input operation failure, like when attempting to read an integer but the data read isn't one
     */
    file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try {
        file.open(filename);
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        return ss.str();
    }
    catch (std::ifstream::failure &e) {
        std::cout << "Cannot load shader file " << filename << std::endl;
        throw e;
    }
}

unsigned int Shader::compileShader(int shaderType, const std::string &filename) {
    unsigned int shader = glCreateShader(shaderType);
    std::string source = getSource(filename);
    const char *sourcePtr = source.c_str();

    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Cannot compile shader file " << filename << std::endl;
        std::cout << infoLog << std::endl;
    }
    return shader;
}

unsigned int Shader::getLocation(const std::string &name) {
    auto it = uniforms.find(name);
    if (it == uniforms.end()) {
        unsigned int location = glGetUniformLocation(program, name.c_str());
        uniforms.insert(std::make_pair(name, location));
        return location;
    }
    else {
        return it->second;
    }
}

void Shader::setMatrix4(const std::string &name, const glm::mat4 &mat) {
    unsigned int location = getLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::use() const {
    glUseProgram(program);
}

void Shader::setInt(const std::string &name, int value) {
    unsigned int location = getLocation(name);
    glUniform1i(location, value);
}
