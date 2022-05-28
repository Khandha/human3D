#include "Skeleton.hpp"

Skeleton::Skeleton(std::unordered_map<std::string, Bone*> bones,
                   std::string parentBoneId) : parent_bone_id(std::move(parentBoneId)), bones(std::move(bones))
{
    this->parent_bone = this->bones[this->parent_bone_id];
}

Skeleton::~Skeleton(void)
{
    delete this->parent_bone;
}

void Skeleton::update(void) const
{
    this->parent_bone->update(mat4(1.0f), this->shader);
}

Bone* Skeleton::operator[](const std::string& id)
{
    return this->bones[id];
}