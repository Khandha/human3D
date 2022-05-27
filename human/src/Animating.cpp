#include "Animating.hpp"
#include "glm/gtx/string_cast.hpp"

Animating::Animating(Skeleton* skeleton, std::vector<anim> animations) : skeleton(skeleton), animations(animations)
{
    this->time_point = std::chrono::steady_clock::now();
    this->current_animation = 0;
    this->current_frame = 0;
    this->current_frame_duration = this->animations[0].duration / this->animations[0].frames->size();
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
    // if id is different from current animation
    if (this->current_animation != id)
    {
        // reset all the parts to external transform (identity matrix)
        // if the animation is in play, stop it
        if (this->animations[current_animation].frames->size() > 1)
        {
            for (auto current : *(*this->animations[current_animation].frames)[this->current_frame])
            {
                (*this->skeleton)[current.bone_id]->get_model()->set_transform(mat4(1.0f));
            }
            this->skeleton->update();
        }
        // update the object with selected animation 
        this->current_animation = id;
        this->current_frame = 0;
        this->current_frame_duration = this->animations[id].duration / this->animations[id].frames->size();
    }
}

void Animating::update(void)
{
    // if the animation frame exceeds the allowed duration
    if (this->get_elapsed_milliseconds().count() > this->current_frame_duration)
    {
        // get next frame
        this->time_point = std::chrono::steady_clock::now();
        this->current_frame = this->get_next_frame();
    }

    // if the animation is not static
    if (this->animations[current_animation].frames->size() > 1)
    {
        // get interpolation for given frame
        const float t = this->get_frame_interpolation();
        mat4 transform;

        // for each bone in the animation
        for (size_t i = 0; i < (*this->animations[current_animation].frames)[this->current_frame]->size(); ++i)
        {
            // get current frame for given bone
            bone_transform curr = (*(*this->animations[current_animation].frames)[this->current_frame])[i];

            //get next frame for given bone
            const bone_transform next = (*(*this->animations[current_animation].frames)[this->get_next_frame()])[i];

            // rescale bone to the next frame 
            (*this->skeleton)[curr.bone_id]->rescale(
                glm::mix(curr.scale, next.scale, t));
            transform = mat4(1.0f);

            // translate and rotate bones to the next frame
            translate(transform, glm::mix(curr.translation, next.translation, t));
            rotate_around_offset(transform, glm::mix(curr.rotation, next.rotation, t),
                         (*this->skeleton)[curr.bone_id]->get_model()->get_joint());

            // transform the bone to the next frame
            (*this->skeleton)[curr.bone_id]->get_model()->set_transform(transform);
        }
    }
    this->skeleton->update();
}

void Animating::handle_keys(const std::array<key, GLFW_KEY_LAST + 1>& keys)
{
    if (keys[GLFW_KEY_W].value)
        this->select_animation(1); // walking
    else this->select_animation(0); // idle (no animation)
}

millisecs Animating::get_elapsed_milliseconds(void) const
{
    return (std::chrono::steady_clock::now() - this->time_point);
}

size_t Animating::get_next_frame(void) const
{
    return (this->current_frame + 1 >= this->animations[current_animation].frames->size() ? 0 : this->current_frame + 1);
}

float Animating::get_frame_interpolation(void) const
{
    // return interpolation factor between two frames
    return (1 - (this->current_frame_duration - this->get_elapsed_milliseconds().count()) / this->current_frame_duration);
}
