#include "Bone.hpp"

Bone::Bone(std::forward_list<Bone*> children, std::string name, const vec3& position, const vec3& orientation,
           const vec3& scale, const vec3& joint, const int64_t color) : name(std::move(name)), children(std::move(children))
{
    this->model = new Model(position, orientation, scale, joint, color);
}

Bone::~Bone(void)
{
    for (auto it = this->children.begin(); it != this->children.end(); ++it)
        delete *it;
    delete this->model;
}

void Bone::rescale(const vec3& v, bool child)
{
    if (child == false)
    {
        // compute the position change for the new scale relative to the joint
        vec3 d = (v - this->model->getScaling()) * 0.5;
        vec3 n = d.multiply(mtls::normalize(this->model->getJoint() - this->model->getPosition()));
        // update the translation/joint/scale values for the model
        this->model->setPosition(this->model->getPosition() - n);
        this->model->setJoint(this->model->getJoint() + n);
        this->model->setScaling(v);
        // apply the changes to the children
        for (auto it = this->children.begin(); it != this->children.end(); ++it)
            if (*it) (*it)->rescale(d, true);
    }
    else
    {
        // the changes on the children are relative to their position (as it is in the parent local-space)
        vec3 n = v.multiply(mtls::normalize(this->model->getJoint() - this->model->getPosition()));
        this->model->setPosition(this->model->getPosition() - n);
    }
}

void Bone::update(const mat4& transform, Shader* shader)
{
    this->model->update(transform, shader);
    const mat4 parentTransform = this->model->popMatrix();
    for (auto it = this->children.begin(); it != this->children.end(); ++it)
        if (*it) (*it)->update(parentTransform, shader);
}
