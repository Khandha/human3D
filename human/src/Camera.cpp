#include "Camera.hpp"
#include "glm/gtx/string_cast.hpp"

Camera::Camera(float fov, float aspect) : fov(fov), aspect(aspect)
{
    this->projectionMatrix = createPerspectiveProjectionMatrix(fov, aspect);
    this->position = vec3({0, 0, 8});
    this->target = vec3({0, 0, 0});
    this->viewMatrix = lookAt(this->position, this->target, vec3(0, 1, 0));
}

Camera::Camera(const Camera& rhs)
{
    *this = rhs;
}

Camera& Camera::operator=(const Camera& rhs)
{
    this->projectionMatrix = rhs.getProjectionMatrix();
    this->viewMatrix = rhs.getViewMatrix();
    this->aspect = rhs.getAspect();
    this->fov = rhs.getFov();
    this->position = rhs.getPosition();
    this->target = rhs.getTarget();
    return (*this);
}

Camera::~Camera(void)
{
}

void Camera::setFov(float fov)
{
    this->fov = fov;
    const float yScale = 1.0 / std::tan(glm::radians(this->fov / 2.0));
    const float xScale = yScale / this->aspect;
    this->projectionMatrix[0][0] = xScale;
    this->projectionMatrix[1][2] = yScale;
}

void Camera::setAspect(float aspect)
{
    this->aspect = aspect;
    const float yScale = 1.0 / std::tan(glm::radians(this->fov / 2.0));
    const float xScale = yScale / this->aspect;
    this->projectionMatrix[0][0] = xScale;
    this->projectionMatrix[1][2] = yScale;
}

mat4 Camera::createPerspectiveProjectionMatrix(float fov, float aspect, float near, float far)
{
    const float yScale = 1 / std::tan(glm::radians(fov * 0.5));
    const float xScale = yScale / aspect;
    const float nearfar = near - far;
    mat4 projectionMatrix(
        xScale, 0, 0, 0,
        0, yScale, 0, 0,
        0, 0, (far + near) / nearfar, -1,
        0, 0, 2 * far * near / nearfar, 0
    );
    return (projectionMatrix);
}

// TODO: leaving this in for reference REMOVE AFTER MOUSE HANDLER CREATION
void Camera::handleKeys(const std::array<tKey, N_KEY>& keys, const vec3& lockPos)
{
    vec4 translate({
        static_cast<float>(keys[GLFW_KEY_A].value - keys[GLFW_KEY_D].value),
        static_cast<float>(keys[GLFW_KEY_LEFT_SHIFT].value - keys[GLFW_KEY_SPACE].value),
        static_cast<float>(keys[GLFW_KEY_W].value - keys[GLFW_KEY_S].value),
        1.0f
    });
    // translation is in the same coordinate system as view (moves in same direction) 
    translate = this->viewMatrix * glm::normalize(translate);
    /* change the target if we are in orbit or free mode */
    if (keys[GLFW_KEY_C].value)
        this->target = this->interpolate(this->target, lockPos, keys[GLFW_KEY_C].stamp, 2000);
    else
        this->target = vec3(0.0f, 0.0f, -2.0f);
    this->viewMatrix = lookAt(this->position, this->target, vec3(0, 1, 0));
}

vec3 Camera::interpolate(const vec3& v0, const vec3& v1, tTimePoint last, size_t duration)
{
    float t = (1 - (static_cast<float>(duration) - this->getElapsedMilliseconds(last).count()) / static_cast<float>(
        duration));
    t = std::min(t, 1.0f);
    return (glm::mix(v0, v1, t));
}

millisecs Camera::getElapsedMilliseconds(tTimePoint last)
{
    return (std::chrono::steady_clock::now() - last);
}
