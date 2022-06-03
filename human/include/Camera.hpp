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
    void handle_keys(const std::array<key, GLFW_KEY_LAST + 1>& keys, const vec3& lockPos);
    static mat4 create_perspective_projection_matrix(float fov, float aspect, float near = 0.1f, float far = 100.0f);
    
    // GETTERS
    const mat4& get_projection_matrix(void) const { return (projection_matrix_); }
    const mat4& get_view_matrix(void) const { return (view_matrix_); }
    const vec3& get_position(void) const { return (position_); }
    
private:
    mat4 projection_matrix_;
    mat4 view_matrix_;
    float fov_;
    float aspect_;
    vec3 position_;
    vec3 target_;
};
