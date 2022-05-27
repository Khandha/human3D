#pragma once

#include <glad/glad.h>

#include "Env.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

class Renderer
{
public:
    Renderer(Env* env);
    ~Renderer(void);

    void loop(void);

private:
    Env* env;
    Shader shader;
    Camera camera;

    void updateShaderUniforms(void);
};
