#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>

#include "Exception.hpp"
#include "Controller.hpp"
#include "Animating.hpp"
#include "Skeleton.hpp"
#include "Bone.hpp"

using t_window = struct s_window
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

    const t_window& getWindow(void) const { return (window); }
    Skeleton* getCharacter(void) const { return (character); }
    Animating* getAnimator(void) const { return (animator); }
    Controller* getController(void) const { return (controller); }

private:
    t_window window;
    Controller* controller;
    Animating* animator;
    Skeleton* character;

    void initGlfwEnvironment(const std::string& glVersion = "4.0") const;
    void initGlfwWindow(size_t width, size_t height);
    void setupController(void) const;
    // callback to be called each time the window is resized to update the viewport size as well
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    static std::unordered_map<std::string, Bone*> createCharacterSkeleton(void);
    static anim_frames* createWalkingAnimation(void);
};
