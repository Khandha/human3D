#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "Exception.hpp"
#include "Skeleton.hpp"
#include "Matrix.hpp"
#include "Bone.hpp"
#include "Controller.hpp"


using bone_transform = struct s_bone_transform
{
    std::string boneId;
    vec3 translation;
    vec3 rotation;
    vec3 scale;
};

using anim_frames = std::vector<std::vector<bone_transform>*>;
using millisecs = std::chrono::duration<double, std::milli>;

using anim = struct s_animation
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
    void handle_keys(const std::array<tKey, N_KEY>& keys);
    size_t get_next_frame(void) const;
    float get_frame_interpolation(void) const;
    millisecs get_elapsed_milliseconds(void) const;

    Skeleton* get_skeleton(void) const { return (skeleton); }
    size_t get_c_anim(void) const { return (cAnim); }
    size_t get_c_frame(void) const { return (cFrame); }
    size_t get_c_frame_duration(void) const { return (cFrameDuration); }

private:
    Skeleton* skeleton;
    std::vector<anim> animations;
    size_t cAnim;
    size_t cFrame;
    size_t cFrameDuration;
    std::chrono::steady_clock::time_point pTimepoint;
};
