#include "Keyboard.hpp"



Keyboard::Keyboard(GLFWwindow* window) : window(window)
{
    std::cout << "{A} / {D} : move camera left or right \n\
{W} : walk \n\
{ESC} : quit \n";
}

Keyboard::~Keyboard(void)
= default;

void Keyboard::update(void)
{
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, GL_TRUE);
    for (size_t k = 0; k < GLFW_KEY_LAST + 1; ++k)
        this->keys[k].value = (glfwGetKey(this->window, k) == GLFW_PRESS);
}
