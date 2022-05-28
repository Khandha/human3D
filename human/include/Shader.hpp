#pragma once

#include <glad/glad.h>

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <forward_list>
#include <unordered_map>

#include "Matrix.hpp"

class Shader
{
public:
    Shader(const std::string& vertexShader, const std::string& fragmentShader);
    ~Shader(void);

    static std::string get_shader_from_file(const std::string& filename);
    GLuint create(const char* shaderSource, GLenum shaderType);
    GLuint create_program(const std::forward_list<GLuint>& shaders);

    void use(void) const;

    unsigned int get_uniform_location(const std::string& name);
    void set_mat4_uniform(const std::string& name, const mat4& m);
    void set_vec4_uniform(const std::string& name, const vec4& v);

    GLuint id;
    
private:
    std::unordered_map<std::string, unsigned int> uniformLocations;
};
