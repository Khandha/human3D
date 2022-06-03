#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <string>
#include <unordered_map>

#include "Bone.hpp"
#include "Shader.hpp"
#include "Keyboard.hpp"

class Skeleton
{
public:
    Skeleton(std::unordered_map<std::string, Bone*> bones, std::string parentBoneId);
    ~Skeleton(void);
    void update(void) const;

    // SETTERS
    void set_shader(Shader* s) { shader = s; }
    
    // GETTERS
    const Bone* get_parent_bone(void) const { return (parent_bone); }
    const std::unordered_map<std::string, Bone*>& get_bones(void) const { return (bones); }
    Bone* operator[](const std::string& id);

private:
    Bone* parent_bone;
    std::string parent_bone_id;
    std::unordered_map<std::string, Bone*> bones;
    Shader* shader;
};
