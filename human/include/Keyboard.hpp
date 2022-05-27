#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <array>

struct key
{
    short value = 0;
};

class Keyboard
{
public:
    Keyboard(GLFWwindow* window);
    ~Keyboard(void);

    void update(void);
    
    // GETTERS
    const std::array<key, GLFW_KEY_LAST + 1>& getKeys(void) const { return (keys); }

private:
    GLFWwindow* window;
    std::array<key, GLFW_KEY_LAST + 1> keys;
};
