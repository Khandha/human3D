#include "Camera.hpp"
#include "glm/gtx/string_cast.hpp"

Camera::Camera(float fov, float aspect) : fov_(fov), aspect_(aspect)
{
    this->projection_matrix_ = create_perspective_projection_matrix(fov, aspect);
    this->position_ = vec3({0, 0, 8}); // tutaj pozycja kamery 
    this->target_ = vec3({0, 0, 0});
    this->view_matrix_ = lookAt(this->position_, this->target_, vec3(0, 1, 0));
}

Camera::~Camera(void)
{
}

mat4 Camera::create_perspective_projection_matrix(float fov, float aspect, float near, float far)
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

void Camera::handle_keys(const std::array<key, GLFW_KEY_LAST + 1>& keys, const vec3& lockPos)
{
    float static rotCamera = 0.0f;
    if (keys[GLFW_KEY_A].value)
    {
        rotCamera += 0.002f;
        glm::vec3 rot(sin(rotCamera) * 8 - this->position_.x,  0, cos(rotCamera) * 8 - this->position_.z);
        this->position_ = vec3(this->position_ + rot);
    } else if (keys[GLFW_KEY_D].value)
    {
        rotCamera -= 0.002f;
        glm::vec3 rot(sin(rotCamera) * 8 - this->position_.x,  0, cos(rotCamera) * 8 - this->position_.z);
        this->position_ = vec3(this->position_ + rot);
    }
    
    this->view_matrix_ = lookAt(this->position_, this->target_, vec3(0, 1, 0));
}