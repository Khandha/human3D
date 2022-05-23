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
    this->parentBone->update(mat4(1.0f), this->shader);
}

Bone* Skeleton::operator[](const std::string& id)
{
    if (this->bones.find(id) == this->bones.end())
        throw Exception::SkeletonMapAccessError(id);
    return (this->bones[id]);
}