#pragma once

#include "Matrix.hpp"
#include <vector>
#include "glad/glad.h"

void create_sphere(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, float radius, unsigned int rings,
                  unsigned int sectors);
void create_cube(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices);
vec4 hex2_vec(int64_t hex);
vec2 mouse_pos_to_clip_space(const vec2d& pos, int winWidth, int winHeight);
