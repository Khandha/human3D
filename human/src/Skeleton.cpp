#include "Skeleton.hpp"

Skeleton::Skeleton(std::unordered_map<std::string, Bone*> bones,
                   std::string parentBoneId) : parentBoneId(std::move(parentBoneId)), bones(std::move(bones))
{
    this->parentBone = this->bones[this->parentBoneId];
}

Skeleton::~Skeleton(void)
{
    delete this->parentBone;
}

void Skeleton::update(void) const
{
    this->parentBone->update(mtls::mat4identity, this->shader);
}

Bone* Skeleton::operator[](const std::string& id)
{
    if (this->bones.find(id) == this->bones.end())
        throw Exception::SkeletonMapAccessError(id);
    return (this->bones[id]);
}

void Skeleton::scaleSelection(const std::array<tKey, N_KEY>& keys, const std::string& boneId)
{
    /* scaling all axis */
    const float gScale = (keys[GLFW_KEY_EQUAL].value - keys[GLFW_KEY_MINUS].value) * 0.05f;
    /* individual axis scaling */
    const vec3 aScale({
        (keys[GLFW_KEY_L].value - keys[GLFW_KEY_J].value) * 0.05f,
        (keys[GLFW_KEY_Y].value - keys[GLFW_KEY_H].value) * 0.05f,
        (keys[GLFW_KEY_I].value - keys[GLFW_KEY_K].value) * 0.05f,
    });
    this->bones[boneId]->getModel()->scaleExternal = mtls::max(
        this->bones[boneId]->getModel()->scaleExternal + aScale + gScale, 0);
    this->bones[boneId]->rescale(mtls::max(this->bones[boneId]->getModel()->getScaling() + aScale + gScale, 0));
    this->update();
}
