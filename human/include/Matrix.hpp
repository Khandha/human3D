#pragma once
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat2x2.hpp>
#include "glm/ext/quaternion_common.hpp"

using mat4 = glm::mat4;
using vec4 = glm::vec4;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
using vec2d = glm::dvec2;

namespace glm
{
    vec3 unit_vector_creator3(size_t index);
    mat4& rotate_around_offset(mat4& m, const vec3& r, const vec3& offset);
}