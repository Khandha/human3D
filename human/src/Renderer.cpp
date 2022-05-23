#include "Renderer.hpp"

Renderer::Renderer(Env* env) :
    //environment initialization
    env(env),
    shader("./shader/vertex.glsl", "./shader/fragment.glsl"),
    camera(50, static_cast<float>(env->getWindow().width) / static_cast<float>(env->getWindow().height))
{
    this->env->getCharacter()->setShader(&this->shader);
}

Renderer::~Renderer(void)
{
}

void Renderer::loop(void)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    while (!glfwWindowShouldClose(this->env->getWindow().ptr)) // main program loop
    {
        glfwPollEvents();

        // background color
        glClearColor(0.858f, 0.854f, 0.843f, 1);

        // clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use custom shader
        this->shader.use();

        // update controller
        this->env->getController()->update();

        // handle animator keys
        this->env->getAnimator()->handle_keys(this->env->getController()->getKeys());

        // update animator
        this->env->getAnimator()->update();

        // handle camera keys
        this->camera.handleKeys(this->env->getController()->getKeys(),
                                this->env->getCharacter()->getParentBone()->getModel()->getPosition());

        // TODO: implement camera mouse movement instead of keyboard (maybe? investigate)
        //this->camera.handleMouse();

        // TODO: what does this do?
        this->updateShaderUniforms();

        // render scene
        glfwSwapBuffers(this->env->getWindow().ptr);
    }
}

void Renderer::updateShaderUniforms(void)
{
    this->shader.setMat4UniformValue("view", this->camera.getViewMatrix());
    this->shader.setMat4UniformValue("projection", this->camera.getProjectionMatrix());
    
    this->shader.setVec4UniformValue("lightPosA", glm::vec4(0,-2,9,1));
    this->shader.setVec4UniformValue("lightColorA", glm::vec4(1,1, 1,1));
    
    this->shader.setVec4UniformValue("lightPosB", glm::vec4(0,2,0,1));
    this->shader.setVec4UniformValue("lightColorB", glm::vec4(0.996, 0.988, 0.784, 1));
    
    this->shader.setVec4UniformValue("viewPos", glm::vec4(this->camera.getPosition(), 1));
}
