#include "Renderer.hpp"
#include <thread>
#include <sstream> 
#define t 10000

Renderer::Renderer(Env* env) :
    //environment initialization
    env(env),
    shader("./shader/vertex.glsl", "./shader/fragment.glsl"),
    camera(50, static_cast<float>(env->get_window().width) / static_cast<float>(env->get_window().height))
{
    this->env->get_character()->set_shader(&this->shader);
}

Renderer::~Renderer(void)
{
}

void Renderer::loop(void)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    std::ostringstream out;
    int i = 0;
    int sum = 0;
    while (!glfwWindowShouldClose(this->env->get_window().ptr)) // main program loop
    {
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
        
        glfwPollEvents();

        // background color
        glClearColor(0.858f, 0.854f, 0.843f, 1);

        // clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use custom shader
        this->shader.use();

        // update keyboard
        this->env->get_keyboard()->update();

        // handle animator keys
        this->env->get_animator()->handle_keys(this->env->get_keyboard()->getKeys());

        // update animator
        this->env->get_animator()->update();

        // handle camera keys
        this->camera.handle_keys(this->env->get_keyboard()->getKeys(),
                                this->env->get_character()->get_parent_bone()->get_model()->get_position());

        // update shader uniforms
        this->updateShaderUniforms();

        // render scene
        glfwSwapBuffers(this->env->get_window().ptr);
        
        std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        
        auto x = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        i++;
        sum += x.count();
        if(i == t/10)
        {
            std::string title =  "Cyborg 3D --- average frame render time: " + std::to_string(sum / (t / 10)) + "µs";
            glfwSetWindowTitle(this->env->get_window().ptr, title.c_str());
        }
        if(i == t)
        {
            out << "Average time: " << sum / t << " microseconds" << std::endl;
            std::cout << out.str();
            i = 0;
            sum = 0;
        }
    }
}

void Renderer::updateShaderUniforms(void)
{
    this->shader.set_mat4_uniform("view", this->camera.get_view_matrix());
    this->shader.set_mat4_uniform("projection", this->camera.get_projection_matrix());
    
    this->shader.set_vec4_uniform("lightPosA", glm::vec4(0,-2,9,1));
    this->shader.set_vec4_uniform("lightColorA", glm::vec4(1,1, 1,1));
    
    this->shader.set_vec4_uniform("lightPosB", glm::vec4(0,2,0,1));
    this->shader.set_vec4_uniform("lightColorB", glm::vec4(0.996, 0.988, 0.784, 1));
    
    this->shader.set_vec4_uniform("viewPos", glm::vec4(this->camera.get_position(), 1));
}
