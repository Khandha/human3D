#include "Env.hpp"

Env::Env(void) : character()
{
    try
    {
        this->init_glfw_environment("4.0");
        this->init_glfw_window(1280, 960);
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            throw std::runtime_error("glad initialization failed");
        
        this->keyboard = new Keyboard(this->window.ptr);
        this->character = new Skeleton(this->create_character_skeleton(), "torso");
        this->animator = new Animating(this->character, {
                                          {new anim_frames({{}}), 200},
                                          {this->create_walking_animation(), 1000},
                                      });
    }
    catch (const std::exception& err)
    {
        std::cout << err.what() << std::endl;
    }
}

Env::~Env(void)
{
    delete this->character;
    delete this->animator;
    delete this->keyboard;
    glfwDestroyWindow(this->window.ptr);
    glfwTerminate();
}

void Env::init_glfw_environment(const std::string& glVersion) const
{
    if (!glfwInit())
        throw std::runtime_error("glfw initialization failed");
    const size_t p = glVersion.find('.');
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, std::stoi(glVersion.substr(0, p)));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, std::stoi(glVersion.substr(p + 1)));
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void Env::init_glfw_window(size_t width, size_t height)
{
    glfwWindowHint(GLFW_SAMPLES, 8);
    if (!(this->window.ptr = glfwCreateWindow(width, height, "Cyborg 3D", nullptr, nullptr)))
        throw std::runtime_error("glfwCreateWindow failed");
    glfwMakeContextCurrent(this->window.ptr);
    glfwSetInputMode(this->window.ptr, GLFW_STICKY_KEYS, 1);
    this->window.width = width;
    this->window.height = height;
}

anim_frames* Env::create_walking_animation(void)
{
    const auto walkingAnimation = new anim_frames({
        {
            // idle
            new std::vector<bone_transform>({
                {
                    {"upper_arm_left", vec3(0, 0, 0), vec3(-0.75f, 0, 0), vec3(0, 0, 0)},
                    {"lower_arm_left", vec3(0, 0, 0), vec3(0.75f, 0, 0), vec3(0, 0, 0)},
                    {"upper_arm_right", vec3(0, 0, 0), vec3(0.7f, 0.9f, -0.1f), vec3(0, 0, 0)},
                    {"lower_arm_right", vec3(0, 0, 0), vec3(0.5, 0, 0), vec3(0, 0, 0)},
                    {"torso", vec3(0, 0, 0), vec3(-0.22f, 0.1f, 0.025f), vec3(0, 0, 0)},
                    {"pelvis", vec3(0, 0, 0), vec3(0.22f, 0, -0.025f), vec3(0, 0, 0)},
                    {"head", vec3(0, 0, 0), vec3(0, 0, -0.05f), vec3(0, 0, 0)},
                    {"thigh_left", vec3(0, 0, 0), vec3(0.75f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_left", vec3(0, 0, 0), vec3(-0.1f, 0, 0), vec3(0, 0, 0)},
                    {"thigh_right", vec3(0, 0, 0), vec3(-0.45f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_right", vec3(0, 0, 0), vec3(-0.25f, 0, 0), vec3(0, 0, 0)},
                }
            }),
            // down
            new std::vector<bone_transform>({
                {
                    {"upper_arm_left", vec3(0, 0, 0), vec3(-0.85f, 0, 0), vec3(0, 0, 0)},
                    {"lower_arm_left", vec3(0, 0, 0), vec3(0.62f, 0, 0), vec3(0, 0, 0)},
                    {"upper_arm_right", vec3(0, 0, 0), vec3(0.81f, 0.5f, -0.05f), vec3(0, 0, 0)},
                    {"lower_arm_right", vec3(0, 0, 0), vec3(0.32f, 0, 0), vec3(0, 0, 0)},
                    {"torso", vec3(0, 0, 0), vec3(-0.24f, 0.24f, 0), vec3(0, 0, 0)},
                    {"pelvis", vec3(0, 0, 0), vec3(0.24f, 0, 0), vec3(0, 0, 0)},
                    {"head", vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0)},
                    {"thigh_left", vec3(0, 0, 0), vec3(0.82f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_left", vec3(0, 0, 0), vec3(-0.82f, 0, 0), vec3(0, 0, 0)},
                    {"thigh_right", vec3(0, 0, 0), vec3(-0.62f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_right", vec3(0, 0, 0), vec3(-0.21f, 0, 0), vec3(0, 0, 0)},
                }
            }),
            // avg
            new std::vector<bone_transform>({
                {
                    {"upper_arm_left", vec3(0, 0, 0), vec3(-0.03f, 0, 0), vec3(0, 0, 0)},
                    {"lower_arm_left", vec3(0, 0, 0), vec3(0.25f, 0, 0), vec3(0, 0, 0)},
                    {"upper_arm_right", vec3(0, 0, 0), vec3(0.3f, 0.2f, 0), vec3(0, 0, 0)},
                    {"lower_arm_right", vec3(0, 0, 0), vec3(0.3f, 0, 0), vec3(0, 0, 0)},
                    {"torso", vec3(0, 0, 0), vec3(-0.2f, 0.1f, -0.03f), vec3(0, 0, 0)},
                    {"pelvis", vec3(0, 0, 0), vec3(0.2f, 0, 0.07f), vec3(0, 0, 0)},
                    {"head", vec3(0, 0, 0), vec3(0, 0, 0.07f), vec3(0, 0, 0)},
                    {"thigh_left", vec3(0, 0, 0), vec3(-0.1f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_left", vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0)},
                    {"thigh_right", vec3(0, 0, 0), vec3(0.5, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_right", vec3(0, 0, 0), vec3(-1.8f, 0, 0), vec3(0, 0, 0)},
                }
            }),
            // going up
            new std::vector<bone_transform>({
                {
                    {"upper_arm_left", vec3(0, 0, 0), vec3(0.42f, 0, 0), vec3(0, 0, 0)},
                    {"lower_arm_left", vec3(0, 0, 0), vec3(0.21f, 0, 0), vec3(0, 0, 0)},
                    {"upper_arm_right", vec3(0, 0, 0), vec3(-0.22f, 0, 0), vec3(0, 0, 0)},
                    {"lower_arm_right", vec3(0, 0, 0), vec3(0.32f, 0, 0), vec3(0, 0, 0)},
                    {"torso", vec3(0, 0, 0), vec3(-0.2f, 0, -0.07f), vec3(0, 0, 0)},
                    {"pelvis", vec3(0, 0, 0), vec3(0.21f, 0, 0.14f), vec3(0, 0, 0)},
                    {"head", vec3(0, 0, 0), vec3(0, 0, 0.13f), vec3(0, 0, 0)},
                    {"thigh_left", vec3(0, 0, 0), vec3(-0.21f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_left", vec3(0, 0, 0), vec3(-0.125f, 0, 0), vec3(0, 0, 0)},
                    {"thigh_right", vec3(0, 0, 0), vec3(0.92f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_right", vec3(0, 0, 0), vec3(-1.25f, 0, 0), vec3(0, 0, 0)},
                }
            }),
            // idle
            new std::vector<bone_transform>({
                {
                    {"upper_arm_left", vec3(0, 0, 0), vec3(0.7f, -0.9f, 0.1f), vec3(0, 0, 0)},
                    {"lower_arm_left", vec3(0, 0, 0), vec3(0.5, 0, 0), vec3(0, 0, 0)},
                    {"upper_arm_right", vec3(0, 0, 0), vec3(-0.7f, 0, 0), vec3(0, 0, 0)},
                    {"lower_arm_right", vec3(0, 0, 0), vec3(0.7f, 0, 0), vec3(0, 0, 0)},
                    {"torso", vec3(0, 0, 0), vec3(-0.2f, -0.1f, -0.025f), vec3(0, 0, 0)},
                    {"pelvis", vec3(0, 0, 0), vec3(0.2f, 0, 0.025f), vec3(0, 0, 0)},
                    {"head", vec3(0, 0, 0), vec3(0, 0, 0.05f), vec3(0, 0, 0)},
                    {"thigh_left", vec3(0, 0, 0), vec3(-0.4f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_left", vec3(0, 0, 0), vec3(-0.2f, 0, 0), vec3(0, 0, 0)},
                    {"thigh_right", vec3(0, 0, 0), vec3(0.7f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_right", vec3(0, 0, 0), vec3(-0.1f, 0, 0), vec3(0, 0, 0)},
                }
            }),
            // down
            new std::vector<bone_transform>({
                {
                    {"upper_arm_left", vec3(0, 0, 0), vec3(0.8f, -0.5f, 0.05f), vec3(0, 0, 0)},
                    {"lower_arm_left", vec3(0, 0, 0), vec3(0.6f, 0, 0), vec3(0, 0, 0)},
                    {"upper_arm_right", vec3(0, 0, 0), vec3(-0.8f, 0, 0), vec3(0, 0, 0)},
                    {"lower_arm_right", vec3(0, 0, 0), vec3(0.3f, 0, 0), vec3(0, 0, 0)},
                    {"torso", vec3(0, 0, 0), vec3(-0.2f, -0.2f, 0), vec3(0, 0, 0)},
                    {"pelvis", vec3(0, 0, 0), vec3(0.2f, 0, 0), vec3(0, 0, 0)},
                    {"head", vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0)},
                    {"thigh_left", vec3(0, 0, 0), vec3(-0.6f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_left", vec3(0, 0, 0), vec3(-0.2f, 0, 0), vec3(0, 0, 0)},
                    {"thigh_right", vec3(0, 0, 0), vec3(0.8f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_right", vec3(0, 0, 0), vec3(-0.8f, 0, 0), vec3(0, 0, 0)},
                }
            }),
            // avg
            new std::vector<bone_transform>({
                {
                    {"upper_arm_left", vec3(0, 0, 0), vec3(0.3f, -0.2f, 0), vec3(0, 0, 0)},
                    {"lower_arm_left", vec3(0, 0, 0), vec3(0.3f, 0, 0), vec3(0, 0, 0)},
                    {"upper_arm_right", vec3(0, 0, 0), vec3(-0.03f, 0, 0), vec3(0, 0, 0)},
                    {"lower_arm_right", vec3(0, 0, 0), vec3(0.25, 0, 0), vec3(0, 0, 0)},
                    {"torso", vec3(0, 0, 0), vec3(-0.2f, -0.1f, 0.03f), vec3(0, 0, 0)},
                    {"pelvis", vec3(0, 0, 0), vec3(0.2f, 0, -0.07f), vec3(0, 0, 0)},
                    {"head", vec3(0, 0, 0), vec3(0, 0, -0.07f), vec3(0, 0, 0)},
                    {"thigh_left", vec3(0, 0, 0), vec3(0.5, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_left", vec3(0, 0, 0), vec3(-1.8f, 0, 0), vec3(0, 0, 0)},
                    {"thigh_right", vec3(0, 0, 0), vec3(-0.1f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_right", vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0)},
                }
            }),
            // up
            new std::vector<bone_transform>({
                {
                    {"upper_arm_left", vec3(0, 0, 0), vec3(-0.2f, 0, 0), vec3(0, 0, 0)},
                    {"lower_arm_left", vec3(0, 0, 0), vec3(0.3f, 0, 0), vec3(0, 0, 0)},
                    {"upper_arm_right", vec3(0, 0, 0), vec3(0.4f, 0, 0), vec3(0, 0, 0)},
                    {"lower_arm_right", vec3(0, 0, 0), vec3(0.2f, 0, 0), vec3(0, 0, 0)},
                    {"torso", vec3(0, 0, 0), vec3(-0.2f, 0, 0.07f), vec3(0, 0, 0)},
                    {"pelvis", vec3(0, 0, 0), vec3(0.2f, 0, -0.13f), vec3(0, 0, 0)},
                    {"head", vec3(0, 0, 0), vec3(0, 0, -0.13f), vec3(0, 0, 0)},
                    {"thigh_left", vec3(0, 0, 0), vec3(0.9f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_left", vec3(0, 0, 0), vec3(-1.2f, 0, 0), vec3(0, 0, 0)},
                    {"thigh_right", vec3(0, 0, 0), vec3(-0.2f, 0, 0), vec3(0, 0, 0)},
                    {"lower_leg_right", vec3(0, 0, 0), vec3(-0.1f, 0, 0), vec3(0, 0, 0)},
                }
            }),

        }
    });
    return (walkingAnimation);
}

std::unordered_map<std::string, Bone*> Env::create_character_skeleton(void)
{
    std::unordered_map<std::string, Bone*> bones;
    bones["head"] = new Bone(
        std::forward_list<Bone*>{{}},
        "head",
        vec3(0, 0.425f, 0),
        vec3(0, 0, 0),
        vec3(0.8f, 0.85f, 0.8f),
        vec3(0, -0.225f, 0)
    );
    bones["neck"] = new Bone(
        std::forward_list<Bone*>{bones["head"]},
        "neck",
        vec3(0, 0.5, 0),
        vec3(0, 0, 0),
        vec3(0.3f, 0.3f, 0.3f),
        vec3(0, 0, 0)
    );
    bones["lower_leg_left"] = new Bone(
        std::forward_list<Bone*>{{}},
        "lower_leg_left",
        vec3(0, -0.5, 0),
        vec3(0, 0, 0),
        vec3(0.4f, 1, 0.4f),
        vec3(0, 0.5, 0)
    );
    bones["lower_leg_right"] = new Bone(
        std::forward_list<Bone*>{{}},
        "lower_leg_right",
        vec3(0, -0.5, 0),
        vec3(0, 0, 0),
        vec3(0.4f, 1, 0.4f),
        vec3(0, 0.5, 0)
    );
    bones["knee_left"] = new Bone(
        std::forward_list<Bone*>{bones["lower_leg_left"]},
        "knee_left",
        vec3(0, -0.5, 0),
        vec3(0, 0, 0),
        vec3(0.3f, 0.3f, 0.3f),
        vec3(0, 0, 0)
    );
    bones["knee_right"] = new Bone(
        std::forward_list<Bone*>{bones["lower_leg_right"]},
        "knee_right",
        vec3(0, -0.5, 0),
        vec3(0, 0, 0),
        vec3(0.3f, 0.3f, 0.3f),
        vec3(0, 0, 0)
    );
    bones["thigh_left"] = new Bone(
        std::forward_list<Bone*>{bones["knee_left"]},
        "thigh_left",
        vec3(0, -0.5f, 0),
        vec3(0, 0, 0),
        vec3(0.5f, 1, 0.5f),
        vec3(0, 0.5f, 0)
    );
    bones["thigh_right"] = new Bone(
        std::forward_list<Bone*>{bones["knee_right"]},
        "thigh_right",
        vec3(0, -0.5f, 0),
        vec3(0, 0, 0),
        vec3(0.5f, 1, 0.5f),
        vec3(0, 0.5, 0)
    );
    bones["hip_left"] = new Bone(
        std::forward_list<Bone*>{bones["thigh_left"]},
        "hip_left",
        vec3(-0.3f, -0.4f, 0),
        vec3(0, 0, 0),
        vec3(0.3f, 0.3f, 0.3f),
        vec3(0, 0, 0)
    );
    bones["hip_right"] = new Bone(
        std::forward_list<Bone*>{bones["thigh_right"]},
        "hip_right",
        vec3(0.3f, -0.4f, 0),
        vec3(0, 0, 0),
        vec3(0.3f, 0.3f, 0.3f),
        vec3(0, 0, 0)
    );
    bones["lower_arm_left"] = new Bone(
        std::forward_list<Bone*>{{}},
        "lower_arm_left",
        vec3(0, -0.4f, 0),
        vec3(0, 0, 0),
        vec3(0.35f, 0.8f, 0.35f),
        vec3(0, 0.4f, 0)
    );
    bones["lower_arm_right"] = new Bone(
        std::forward_list<Bone*>{{}},
        "lower_arm_right",
        vec3(0, -0.4f, 0),
        vec3(0, 0, 0),
        vec3(0.35f, 0.8f, 0.35f),
        vec3(0, 0.4f, 0)
    );
    bones["elbow_left"] = new Bone(
        std::forward_list<Bone*>{bones["lower_arm_left"]},
        "elbow_left",
        vec3(0, -0.5, 0),
        vec3(0, 0, 0),
        vec3(0.3f, 0.3f, 0.3f),
        vec3(0, 0, 0)
    );
    bones["elbow_right"] = new Bone(
        std::forward_list<Bone*>{bones["lower_arm_right"]},
        "elbow_right",
        vec3(0, -0.5, 0),
        vec3(0, 0, 0),
        vec3(0.3f, 0.3f, 0.3f),
        vec3(0, 0, 0)
    );
    bones["upper_arm_left"] = new Bone(
        std::forward_list<Bone*>{bones["elbow_left"]},
        "upper_arm_left",
        vec3(0, -0.45f, 0),
        vec3(0, 0, 0),
        vec3(0.4f, 0.9f, 0.4f),
        vec3(0, 0.45f, 0)
    );
    bones["upper_arm_right"] = new Bone(
        std::forward_list<Bone*>{bones["elbow_right"]},
        "upper_arm_right",
        vec3(0, -0.45f, 0),
        vec3(0, 0, 0),
        vec3(0.4f, 0.9f, 0.4f),
        vec3(0, 0.45f, 0)
    );
    bones["shoulder_left"] = new Bone(
        std::forward_list<Bone*>{bones["upper_arm_left"]},
        "shoulder_left",
        vec3(-0.85f, 0, 0),
        vec3(0, 0, 0),
        vec3(0.3f, 0.3f, 0.3f),
        vec3(0, 0, 0)
    );
    bones["shoulder_right"] = new Bone(
        std::forward_list<Bone*>{bones["upper_arm_right"]},
        "shoulder_right",
        vec3(0.85f, 0, 0),
        vec3(0, 0, 0),
        vec3(0.3f, 0.3f, 0.3f),
        vec3(0, 0, 0)
    );
    bones["pelvis"] = new Bone(
        std::forward_list<Bone*>{{bones["hip_left"], bones["hip_right"]}},
        "pelvis",
        vec3(0, -0.425f, 0),
        vec3(0, 0, 0),
        vec3(0.95f, 0.95f, 0.8f),
        vec3(0, 0, 0)
    );
    bones["stomach"] = new Bone(
        std::forward_list<Bone*>{bones["pelvis"]},
        "stomach",
        vec3(0, -0.5f, 0),
        vec3(0, 0, 0),
        vec3(0.8f, 0.8f, 0.9f),
        vec3(0, 0, 0)
    );
    bones["torso"] = new Bone(
        std::forward_list<Bone*>{{bones["neck"], bones["stomach"], bones["shoulder_left"], bones["shoulder_right"]}},
        "torso",
        vec3(0, 0.25f, 0),
        vec3(0, 0, 0),
        vec3({1.5f, 1, 1}),
        vec3(0, 0, 0)
    );
    return (bones);
}
