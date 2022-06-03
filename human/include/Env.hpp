#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <unordered_map>
#include "Keyboard.hpp"
#include "Animating.hpp"
#include "Skeleton.hpp"
#include "Bone.hpp"

struct window_current
{
    GLFWwindow* ptr;
    int width;
    int height;
};

class Env
{
public:
    Env(void);
    ~Env(void);

    const window_current& get_window(void) const { return (window); }
    Skeleton* get_character(void) const { return (character); }
    Animating* get_animator(void) const { return (animator); }
    Keyboard* get_keyboard(void) const { return (keyboard); }

private:
    window_current window;
    Keyboard* keyboard;
    Animating* animator;
    Skeleton* character;

    void init_glfw_environment(const std::string& glVersion = "4.0") const;
    void init_glfw_window(size_t width, size_t height);

    static std::unordered_map<std::string, Bone*> create_character_skeleton(void);
    static anim_frames* create_walking_animation(void);
};
