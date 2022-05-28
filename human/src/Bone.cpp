#include "Bone.hpp"

#include "glm/gtx/string_cast.hpp"

Bone::Bone(std::forward_list<Bone*> children, std::string name, const vec3& position, const vec3& orientation,
           const vec3& scale, const vec3& joint) : name(std::move(name)),
                                                                        children(std::move(children))
{
    this->model = new Model(position, orientation, scale, joint, this->name);
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
        const vec3 d = (v - this->model->get_scaling()) * 0.5f;
        const vec3 n = d * normalize(this->model->get_joint() - this->model->get_position());

        // update the translation/joint/scale values for the model
        this->model->set_position(this->model->get_position() - n);
        this->model->set_joint(this->model->get_joint() + n);
        this->model->set_scaling(v);

        // apply the changes to the children
        for (auto it = this->children.begin(); it != this->children.end(); ++it)
            if (*it) (*it)->rescale(d, true);
    }
    else
    {
        // the changes on the children are relative to their position (as it is in the parent local-space)
        const vec3 n = v * normalize(this->model->get_joint() - this->model->get_position());
        this->model->set_position(this->model->get_position() - n);
    }
}

void Bone::update(const mat4& transform, Shader* shader)
{
    this->model->update(transform, shader);
    const mat4 parent_transform = this->model->popMatrix();
    for (auto it = this->children.begin(); it != this->children.end(); ++it)
        if (*it) (*it)->update(parent_transform, shader);
}
