#include "Shader.hpp"

#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
    const std::string vSrc = getFromFile(vertexShader);
    const std::string fSrc = getFromFile(fragmentShader);

    GLuint vertShader = this->create(vSrc.c_str(), GL_VERTEX_SHADER);
    GLuint fragShader = this->create(fSrc.c_str(), GL_FRAGMENT_SHADER);
    this->id = this->createProgram({{vertShader, fragShader}});

}

Shader::~Shader(void)
{
}

void Shader::use(void) const
{
    glUseProgram(this->id);
}

std::string Shader::getFromFile(const std::string& filename)
{
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return (content);
}

GLuint Shader::create(const char* shaderSource, GLenum shaderType)
{
    GLint success;
    const GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    this->isCompilationSuccess(shader, success, shaderType);
    return (shader);
}

GLuint Shader::createProgram(const std::forward_list<GLuint>& shaders)
{
    GLint success;
    const GLuint shaderProgram = glCreateProgram();
    for (auto it = shaders.begin(); it != shaders.end(); ++it)
        glAttachShader(shaderProgram, *it);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    this->isCompilationSuccess(shaderProgram, success, -1);
    for (auto it = shaders.begin(); it != shaders.end(); ++it)
        glDeleteShader(*it);
    return (shaderProgram);
}

void Shader::isCompilationSuccess(GLint handle, GLint success, int shaderType)
{
    if (!success)
    {
        char infoLog[512];
        if (shaderType != -1)
            glGetShaderInfoLog(handle, 512, nullptr, infoLog);
        else
            glGetProgramInfoLog(handle, 512, nullptr, infoLog);
        throw Exception::ShaderError(shaderType, infoLog);
    }
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
    if (this->uniformLocations.find(name) != this->uniformLocations.end())
        return (this->uniformLocations[name]);
    const unsigned int newLoc = glGetUniformLocation(this->id, name.c_str());
    this->uniformLocations[name] = newLoc;
    return (newLoc);
}

void Shader::setMat4UniformValue(const std::string& name, const mat4& m)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value_ptr(m));
}

void Shader::setVec4UniformValue(const std::string& name, const vec4& v)
{
    glUniform4fv(getUniformLocation(name), 1, value_ptr(v));
}
