#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "Skeleton.hpp"
#include "Matrix.hpp"
#include "Bone.hpp"
#include "Keyboard.hpp"


struct bone_transform
{
    std::string bone_id;
    vec3 translation;
    vec3 rotation;
    vec3 scale;
};

using anim_frames = std::vector<std::vector<bone_transform>*>;
using millisecs = std::chrono::duration<double, std::milli>;

struct anim
{
    anim_frames* frames;
    size_t duration;
};

class Animating
{
public:
    Animating(Skeleton* skeleton, std::vector<anim> animations);
    ~Animating(void);

    void select_animation(size_t id);
    void update(void);
    void handle_keys(const std::array<key, GLFW_KEY_LAST + 1>& keys);
    size_t get_next_frame(void) const;
    float get_frame_interpolation(void) const;
    millisecs get_elapsed_milliseconds(void) const;


private:
    Skeleton* skeleton;
    std::vector<anim> animations;
    size_t current_animation;
    size_t current_frame;
    size_t current_frame_duration;
    std::chrono::steady_clock::time_point time_point;
};
