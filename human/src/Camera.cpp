#include "Camera.hpp"
#include "glm/gtx/string_cast.hpp"

Camera::Camera(float fov, float aspect) : fov(fov), aspect(aspect)
{
    this->projectionMatrix = createPerspectiveProjectionMatrix(fov, aspect);
    this->position = vec3({0, 0, 8}); // tutaj pozycja kamery 
    this->target = vec3({0, 0, 0});
    this->viewMatrix = lookAt(this->position, this->target, vec3(0, 1, 0));
}

Camera::~Camera(void)
{
}

mat4 Camera::createPerspectiveProjectionMatrix(float fov, float aspect, float near, float far)
{
    const float yScale = 1 / std::tan(glm::radians(fov * 0.5));
    const float xScale = yScale / aspect;
    return mat4(
        xScale, 0, 0, 0,
        0, yScale, 0, 0,
        0, 0, (far + near) / (near - far), -1,
        0, 0, 2 * far * near / (near - far), 0
    );
}

void Camera::handleKeys(const std::array<key, GLFW_KEY_LAST + 1>& keys, const vec3& lockPos)
{
    float static rotCamera = 0.0f;
    if (keys[GLFW_KEY_A].value)
    {
        rotCamera += 0.002f;
        glm::vec3 rot(sin(rotCamera) * 8 - this->position.x,  0, cos(rotCamera) * 8 - this->position.z);
        this->position = vec3(this->position + rot);
    } else if (keys[GLFW_KEY_D].value)
    {
        rotCamera -= 0.002f;
        glm::vec3 rot(sin(rotCamera) * 8 - this->position.x,  0, cos(rotCamera) * 8 - this->position.z);
        this->position = vec3(this->position + rot);
    }
    
    this->viewMatrix = lookAt(this->position, this->target, vec3(0, 1, 0));
}