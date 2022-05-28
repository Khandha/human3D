#include "Shader.hpp"

#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
    const std::string vertex_file = get_shader_from_file(vertexShader);
    const std::string fragment_file = get_shader_from_file(fragmentShader);

    GLuint vertex_shader = this->create(vertex_file.c_str(), GL_VERTEX_SHADER);
    GLuint fragment_shader = this->create(fragment_file.c_str(), GL_FRAGMENT_SHADER);
    this->id = this->create_program({{vertex_shader, fragment_shader}});
}

Shader::~Shader(void)
{
}

void Shader::use(void) const
{
    glUseProgram(this->id);
}

std::string Shader::get_shader_from_file(const std::string& filename)
{
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return (content);
}

GLuint Shader::create_program(const std::forward_list<GLuint>& shaders)
{
    GLint success;
    const GLuint shaderProgram = glCreateProgram();
    for (auto it = shaders.begin(); it != shaders.end(); ++it)
        glAttachShader(shaderProgram, *it);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char data[512];
        glGetShaderInfoLog(shaderProgram, 512, nullptr, data);
        throw std::runtime_error(data);
    }
    for (auto it = shaders.begin(); it != shaders.end(); ++it)
        glDeleteShader(*it);
    return (shaderProgram);
}

GLuint Shader::create(const char* shaderSource, GLenum shaderType)
{
    GLint success;
    const GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char data[512];
        glGetShaderInfoLog(shader, 512, nullptr, data);
        throw std::runtime_error(data);
    }
    return (shader);
}


unsigned int Shader::get_uniform_location(const std::string& name)
{
    return glGetUniformLocation(this->id, name.c_str());
}

void Shader::set_mat4_uniform(const std::string& name, const mat4& m)
{
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, value_ptr(m));
}

void Shader::set_vec4_uniform(const std::string& name, const vec4& v)
{
    glUniform4fv(get_uniform_location(name), 1, value_ptr(v));
}
