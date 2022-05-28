#include "Model.hpp"

#include "utils.hpp"
#include "glm/gtx/string_cast.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Model::Model(const vec3& position, const vec3& orientation, const vec3& scale, const vec3& joint, std::string name) :
    position(position), orientation(orientation), scale(scale), joint(joint), name(name)
{
    this->number_of_indices = 0;
    this->init_buffer_objects(GL_STATIC_DRAW, eModelType::cube);
    this->pushMatrix(mat4(1.0f));
    this->transform_ = mat4(1.0f);
    this->world_position = vec3(0, 0, 0);
    this->selected = false;
}

Model::~Model(void)
{
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(1, &this->ebo);
    while (!this->stack.empty())
        this->stack.pop();
}

mat4 Model::popMatrix(void)
{
    mat4 top = this->stack.top();
    this->stack.pop();
    return (top);
};

void Model::update(const mat4& parentTransform, Shader* shader)
{
    this->pushMatrix();
    this->stack.top() = translate(this->stack.top(), this->position);
    this->stack.top() = rotate_around_offset(this->stack.top(), this->orientation, this->joint);
    this->stack.top() = parentTransform * this->stack.top() * this->transform_;
    this->pushMatrix();
    this->stack.top() = glm::scale(this->stack.top(), this->scale + this->scaling);
    this->render(shader);
    this->popMatrix();
}

void Model::render(Shader* shader)
{
    const vec4 color = hex2_vec(0xFFFFFF);
    shader->set_vec4_uniform("customColor", color);
    shader->set_mat4_uniform("model", this->stack.top());
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->texture_sm);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->texture_nl);
    
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, this->number_of_indices);
}

void Model::init_buffer_objects(int mode, eModelType modelType)
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    switch (modelType)
    {
    case eModelType::cube: create_cube(vertices, indices);
        break;
    default: break;
    }
    this->number_of_indices = indices.size();

    // gen buffers and vertex arrays
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);

    // bind vertex array object, basically this is an object to allow us to not redo all of this process each time
    // we want to draw an object to screen, all the states we set are stored in the VAO
    glBindVertexArray(this->vao);

    // copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), mode);
    
    glActiveTexture(GL_TEXTURE0);
    this->texture = this->load_texture(this->name.compare("head") == 0 ? "textures/1.bmp" : "textures/6.bmp");
    
    glActiveTexture(GL_TEXTURE1);
    this->texture_sm = this->load_texture(this->name.compare("head") == 0 ? "textures/1_sm.bmp" : "textures/6_sm.bmp");

    glActiveTexture(GL_TEXTURE2);
    this->texture_nl = this->load_texture(this->name.compare("head") == 0 ? "textures/1_normal.bmp" : "textures/6_normal.bmp");

    // set the vertex attribute pointers
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

 
unsigned int Model::load_texture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}