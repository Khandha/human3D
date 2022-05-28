#pragma once

#include <glad/glad.h>
#include <chrono>

#include "Matrix.hpp"
#include "Keyboard.hpp"

using millisecs = std::chrono::duration<double, std::milli>;
using tTimePoint = std::chrono::steady_clock::time_point;

class Camera
{
public:
    Camera(float fov, float aspect);
    ~Camera(void);
    void handleKeys(const std::array<key, GLFW_KEY_LAST + 1>& keys, const vec3& lockPos);
    static mat4 createPerspectiveProjectionMatrix(float fov, float aspect, float near = 0.1f, float far = 100.0f);
    
    // GETTERS
    const mat4& getProjectionMatrix(void) const { return (projectionMatrix); }
    const mat4& getViewMatrix(void) const { return (viewMatrix); }
    const vec3& getPosition(void) const { return (position); }
    
private:
    mat4 projectionMatrix;
    mat4 viewMatrix;
    float fov;
    float aspect;
    vec3 position;
    vec3 target;
};
