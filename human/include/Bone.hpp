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
         const vec3& scale, const vec3& joint = vec3({0, 0, 0}), int64_t color = 0xFFFFFF);
    ~Bone(void);

    void update(const mat4& transform, Shader* shader);
    void rescale(const vec3& v, bool child = false);

    const std::string& getName(void) const { return (name); }
    Model* getModel(void) const { return (model); }
    const std::forward_list<Bone*>& getChildren(void) const { return (children); }

private:
    std::string name;
    Model* model;
    std::forward_list<Bone*> children;
};
