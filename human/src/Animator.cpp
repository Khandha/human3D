#include "Animator.hpp"

Animator::Animator(Skeleton* skeleton, std::vector<tAnimation> animations) : skeleton(skeleton), animations(animations)
{
    if (animations.empty())
        throw Exception::RuntimeError("Animator should contain at least one tAnimation");
    this->pTimepoint = std::chrono::steady_clock::now();
    this->cAnim = 0;
    this->cFrame = 0;
    this->cFrameDuration = this->animations[0].cycleDuration / this->animations[0].frames->size();
    this->selectAnim(0);
}

// destructor
Animator::~Animator(void)
{
    for (const auto& animation : this->animations)
    {
        for (const auto& frame : *animation.frames)
            delete frame;
        delete animation.frames;
    }
}

void Animator::selectAnim(size_t id)
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
                (*this->skeleton)[curr.boneId]->getModel()->setExternalTransform(mtls::mat4identity);
            }
            this->skeleton->update();
        }
        // update the object with selected animation 
        this->cAnim = id;
        this->cFrame = 0;
        this->cFrameDuration = this->animations[id].cycleDuration / this->animations[id].frames->size();
    }
}

void Animator::update(void)
{
    // if the animation frame exceeds the allowed duration
    if (this->getElapsedMilliseconds().count() > this->cFrameDuration)
    {
        // get next frame
        this->pTimepoint = std::chrono::steady_clock::now();
        this->cFrame = this->getNextFrame();
    }

    // if the animation is not static
    if (this->animations[cAnim].frames->size() > 1)
    {
        // get motion interpolation for given frame
        const float t = this->getFrameInterpolation();
        mat4 transform;

        // for each bone in the animation
        for (size_t i = 0; i < (*this->animations[cAnim].frames)[this->cFrame]->size(); ++i)
        {
            // get current frame for given bone
            tBoneTransform curr = (*(*this->animations[cAnim].frames)[this->cFrame])[i];

            //get next frame for given bone
            const tBoneTransform next = (*(*this->animations[cAnim].frames)[this->getNextFrame()])[i];

            // rescale bone to the next frame 
            (*this->skeleton)[curr.boneId]->rescale(
                (*this->skeleton)[curr.boneId]->getModel()->scaleExternal + mtls::lerp(curr.scale, next.scale, t));
            transform.identity();

            // translate and rotate bones to the next frame
            mtls::translate(transform, mtls::lerp(curr.translation, next.translation, t));
            mtls::rotate(transform, mtls::lerp(curr.rotation, next.rotation, t),
                         (*this->skeleton)[curr.boneId]->getModel()->getJoint());

            // transform the bone to the next frame
            (*this->skeleton)[curr.boneId]->getModel()->setExternalTransform(transform);
        }
    }
    this->skeleton->update();
}

void Animator::handleKeys(const std::array<tKey, N_KEY>& keys)
{
    //if (keys[GLFW_KEY_1].value)
    //    this->selectAnim(0); // idle (no animation)
   // else if (keys[GLFW_KEY_W].value) //changed 2 to W
    //    this->selectAnim(1); // walking
    
    if (keys[GLFW_KEY_W].value)
       this->selectAnim(1); // walking
    else this->selectAnim(0); // idle (no animation)
}

tMilliseconds Animator::getElapsedMilliseconds(void) const
{
    return (std::chrono::steady_clock::now() - this->pTimepoint);
}

size_t Animator::getNextFrame(void) const
{
    return (this->cFrame + 1 >= this->animations[cAnim].frames->size() ? 0 : this->cFrame + 1);
}

float Animator::getFrameInterpolation(void) const
{
    // return interpolation factor between two frames
    return (1 - (this->cFrameDuration - this->getElapsedMilliseconds().count()) / this->cFrameDuration);
}
