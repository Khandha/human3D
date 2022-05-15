#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <cmath>
#include <iostream>
#include <string>
#include <array>
#include <chrono>

#include "Exception.hpp"
#include "Matrix.hpp"

#define N_KEY GLFW_KEY_LAST + 1
#define N_MOUSE_BUTTON GLFW_MOUSE_BUTTON_LAST + 1

using millisecs = std::chrono::duration<double, std::milli>;
using tTimePoint = std::chrono::steady_clock::time_point;

enum class eKeyMode
{
    press,
    /* default */
    toggle,
    /* press to switch the key state */
    cooldown,
    /* press and the state remains on for so long */
    instant,
    /* press and the state remains for a frame, wait cooldown to press again */
    cycle,
    /* press and the value cycles from 0 to n */
};

using tKey = struct sKey
{
    short value = 0;
    eKeyMode type = eKeyMode::press;
    GLuint cooldown = 250;
    GLuint cycles = 1;
    tTimePoint last = std::chrono::steady_clock::now();
    tTimePoint stamp = std::chrono::steady_clock::now();
};

using tMouse = struct sMouse
{
    vec2d pos;
    std::array<short, N_MOUSE_BUTTON> button;
};

class Controller
{
public:
    Controller(GLFWwindow* window);
    ~Controller(void);

    void update(void);
    void setKeyProperties(int k, eKeyMode type, short sval, GLuint cooldown = 250, GLuint cycles = 1);
    /* getters */
    short getKeyValue(int k) const { return (key[k].value); }
    short getMouseButtonValue(int b) const { return (mouse.button[b]); }
    const vec2d& getMousePosition(void) const { return (mouse.pos); }

    const std::array<tKey, N_KEY>& getKeys(void) const { return (key); }

private:
    GLFWwindow* window;
    std::array<tKey, N_KEY> key;
    tTimePoint ref;
    tMouse mouse;

    void mouseHandler(void);
    void keyHandler(void);
    void keyUpdate(int k);
    void keyToggle(int k, short value);
    void keyCooldown(int k, short value);
    void keyInstant(int k, short value);
    void keyCycle(int k, short value);
    static millisecs getElapsedMilliseconds(tTimePoint prev);
};
