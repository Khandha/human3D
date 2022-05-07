#include "Renderer.hpp"
#include "Env.hpp"

int main(void)
{
    try
    {
        Env environment; // evironment initialization
        Renderer renderer(&environment); // renderer initialization
        renderer.loop(); //main loop
    }
    catch (const std::exception& err)
    {
        std::cout << err.what() << std::endl;
        return (1);
    }
    return (0);
}
