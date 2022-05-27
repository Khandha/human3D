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

    const window_current& getWindow(void) const { return (window); }
    Skeleton* getCharacter(void) const { return (character); }
    Animating* getAnimator(void) const { return (animator); }
    Keyboard* getKeyboard(void) const { return (keyboard); }

private:
    window_current window;
    Keyboard* keyboard;
    Animating* animator;
    Skeleton* character;

    void initGlfwEnvironment(const std::string& glVersion = "4.0") const;
    void initGlfwWindow(size_t width, size_t height);

    static std::unordered_map<std::string, Bone*> createCharacterSkeleton(void);
    static anim_frames* createWalkingAnimation(void);
};
