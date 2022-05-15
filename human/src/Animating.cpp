#include "Animating.hpp"
#include "glm/gtx/string_cast.hpp"

Animating::Animating(Skeleton* skeleton, std::vector<anim> animations) : skeleton(skeleton), animations(animations)
{
    if (animations.empty())
        throw Exception::RuntimeError("Animator should contain at least one tAnimation");
    this->pTimepoint = std::chrono::steady_clock::now();
    this->cAnim = 0;
    this->cFrame = 0;
    this->cFrameDuration = this->animations[0].duration / this->animations[0].frames->size();
    this->select_animation(0);
}

// destructor
Animating::~Animating(void)
{
    for (const auto& animation : this->animations)
    {
        for (const auto& frame : *animation.frames)
            delete frame;
        delete animation.frames;
    }
}

void Animating::select_animation(size_t id)
{
    // if id is out of range
    if (id >= this->animations.size())
        id = this->animations.size() - 1;

    // if id is different from current animation
    if (this->cAnim != id)
    {
        // reset all the parts to external transform (identity matrix)
        // if the animation is in play, stop it
        if (this->animations[cAnim].frames->size() > 1)
        {
            for (auto curr : *(*this->animations[cAnim].frames)[this->cFrame])
            {
                (*this->skeleton)[curr.boneId]->getModel()->setExternalTransform(mat4(1.0f));
            }
            this->skeleton->update();
        }
        // update the object with selected animation 
        this->cAnim = id;
        this->cFrame = 0;
        this->cFrameDuration = this->animations[id].duration / this->animations[id].frames->size();
    }
}

void Animating::update(void)
{
    // if the animation frame exceeds the allowed duration
    if (this->get_elapsed_milliseconds().count() > this->cFrameDuration)
    {
        // get next frame
        this->pTimepoint = std::chrono::steady_clock::now();
        this->cFrame = this->get_next_frame();
    }

    // if the animation is not static
    if (this->animations[cAnim].frames->size() > 1)
    {
        // get motion interpolation for given frame
        const float t = this->get_frame_interpolation();
        mat4 transform;

        // for each bone in the animation
        for (size_t i = 0; i < (*this->animations[cAnim].frames)[this->cFrame]->size(); ++i)
        {
            // get current frame for given bone
            bone_transform curr = (*(*this->animations[cAnim].frames)[this->cFrame])[i];

            //get next frame for given bone
            const bone_transform next = (*(*this->animations[cAnim].frames)[this->get_next_frame()])[i];

            // rescale bone to the next frame 
            (*this->skeleton)[curr.boneId]->rescale(
                (*this->skeleton)[curr.boneId]->getModel()->scaleExternal + glm::mix(curr.scale, next.scale, t));
            transform = mat4(1.0f);

            // translate and rotate bones to the next frame
            glm::translate(transform, glm::mix(curr.translation, next.translation, t));
            glm::rotate_around_offset(transform, glm::mix(curr.rotation, next.rotation, t),
                         (*this->skeleton)[curr.boneId]->getModel()->getJoint());

            // transform the bone to the next frame
            (*this->skeleton)[curr.boneId]->getModel()->setExternalTransform(transform);
        }
    }
    this->skeleton->update();
}

void Animating::handle_keys(const std::array<tKey, N_KEY>& keys)
{
    if (keys[GLFW_KEY_1].value)
        this->select_animation(0); // idle (no animation)
    else if (keys[GLFW_KEY_2].value)
        this->select_animation(1); // walking
}

millisecs Animating::get_elapsed_milliseconds(void) const
{
    return (std::chrono::steady_clock::now() - this->pTimepoint);
}

size_t Animating::get_next_frame(void) const
{
    return (this->cFrame + 1 >= this->animations[cAnim].frames->size() ? 0 : this->cFrame + 1);
}

float Animating::get_frame_interpolation(void) const
{
    // return interpolation factor between two frames
    // return 0.5f;
    return (1 - (this->cFrameDuration - this->get_elapsed_milliseconds().count()) / this->cFrameDuration);
}
