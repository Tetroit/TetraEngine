#include "tetrapc.h"
#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

using namespace TetraEngine;

Shader* Shader::billboardShader = nullptr;
Shader* Shader::skyboxShader = nullptr;
Shader* Shader::skysphereShader = nullptr;
Shader* Shader::textShader = nullptr;
Shader* Shader::currentShader = nullptr;

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) : vertexPath(vertexPath), fragmentPath(fragmentPath)
{
	std::string vertexCode;
	std::ifstream vShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string fragmentCode;
    std::ifstream fShaderFile;
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        //vertex shader
        vShaderFile.open(vertexPath);
        std::stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();
        vertexCode = vShaderStream.str();
        //fragment shader
        fShaderFile.open(fragmentPath);
        std::stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ::" << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();



    //Compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    currentShader = this;
    glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const 
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetMat4(const std::string& name, glm::mat4& value) const
{
    unsigned int loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}
void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
    unsigned int loc = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(loc, 1, &value[0]);
}
void Shader::SetVec4(const std::string& name, glm::vec4& value) const
{
    unsigned int loc = glGetUniformLocation(ID, name.c_str());
    glUniform4fv(loc, 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const {

    unsigned int loc = glGetUniformLocation(ID, name.c_str());
    glUniform3f(loc, x, y, z);
}
void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const {

    unsigned int loc = glGetUniformLocation(ID, name.c_str());
    glUniform4f(loc, x, y, z, w);
}

uint Shader::GetID() const {
    return ID;
}

std::string Shader::GetVertexPath() const {
    return vertexPath;
}

std::string Shader::GetFragmentPath() const {
    return fragmentPath;
}

std::string Shader::VertexAsString() const {
    std::ifstream file(vertexPath);
    if (!file.is_open())
        return "Error: could not open " + vertexPath;

    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::string Shader::FragmentAsString() const {

    std::ifstream file(fragmentPath);
    if (!file.is_open())
        return "Error: could not open " + fragmentPath;

    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

}

void Shader::InitialiseShaders() {
    billboardShader = new Shader(shaderPath + "/billboard.glvs", shaderPath + "/billboard.glfs");
    skyboxShader = new Shader(shaderPath + "/skybox.glvs", shaderPath + "/skybox.glfs");
    skysphereShader = new Shader(shaderPath + "/skysphere.glvs", shaderPath + "/skysphere.glfs");
    textShader = new Shader(shaderPath + "/text.glvs", shaderPath + "/text.glfs");
}
