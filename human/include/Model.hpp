#pragma once

#include <glad/glad.h>
#include <stack>

#include "Matrix.hpp"
#include "Shader.hpp"


enum class eModelType
{
    cube,
    sphere,
    cylinder
};

class Model
{
public:
    Model(const vec3& position, const vec3& orientation, const vec3& scale, const vec3& joint, std::string name);
    ~Model(void);

    void update(const mat4& parentTransform, Shader* shader);
    void render(Shader* shader);

    mat4 popMatrix(void);
    void pushMatrix(void) { stack.push(stack.top()); }
    void pushMatrix(const mat4& mat) { stack.push(mat); }

    // GETTERS
    const vec3& get_position(void) const { return (position); }
    const vec3& get_joint(void) const { return (joint); }
    const vec3& get_scaling(void) const { return (scaling); }
    
    // SETTERS
    void set_transform(const mat4& transform) { transform_ = transform; }
    void set_position(const vec3& t) { position = t; }
    void set_joint(const vec3& j) { joint = j; }
    void set_scaling(const vec3& s) { scaling = s; }


private:
    int number_of_indices;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    std::stack<mat4> stack;
    mat4 transform_;

    vec3 position;
    vec3 orientation;
    vec3 scale;
    vec3 world_position;

    vec3 joint;
    vec3 scaling;

    bool selected;

    std::string name;

    GLuint texture;
    GLuint texture_sm;
    GLuint texture_nl;
    GLuint qrcode;

    void init_buffer_objects(int mode = GL_STATIC_DRAW);
    unsigned load_texture(char const* path);
};
