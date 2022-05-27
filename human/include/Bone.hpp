#pragma once

#include <string>
#include <forward_list>
#include "Model.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"

class Bone
{
public:
    Bone(std::forward_list<Bone*> children, std::string name, const vec3& position, const vec3& orientation,
         const vec3& scale, const vec3& joint = vec3(0.0f, 0.0f, 0.0f));
    ~Bone(void);

    void update(const mat4& transform, Shader* shader);
    void rescale(const vec3& v, bool child = false);
    Model* get_model(void) const { return (model); }

private:
    std::string name;
    Model* model;
    std::forward_list<Bone*> children;
};
