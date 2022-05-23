#include "Env.hpp"

Env::Env(void) : character()
{
    try
    {
        // Basic initialization of environment
        
        this->initGlfwEnvironment("4.0");
        this->initGlfwWindow(1280, 960);
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            throw Exception::InitError("glad initialization failed");
        this->controller = new Controller(this->window.ptr);
        this->character = new Skeleton(this->createCharacterSkeleton(), "torso");
        this->animator = new Animating(this->character, {
                                          {new anim_frames({{}}), 200},
                                          {this->createWalkingAnimation(), 1000},
                                      });
        this->setupController();
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
    delete this->controller;
    glfwDestroyWindow(this->window.ptr);
    glfwTerminate();
}

void Env::initGlfwEnvironment(const std::string& glVersion) const
{
    if (!glfwInit())
        throw Exception::InitError("glfw initialization failed");
    const size_t p = glVersion.find('.');
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, std::stoi(glVersion.substr(0, p)));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, std::stoi(glVersion.substr(p + 1)));
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void Env::initGlfwWindow(size_t width, size_t height)
{
    glfwWindowHint(GLFW_SAMPLES, 8);
    if (!(this->window.ptr = glfwCreateWindow(width, height, "Cyborg 3D", nullptr, nullptr)))
        throw Exception::InitError("glfwCreateWindow failed");
    glfwMakeContextCurrent(this->window.ptr);
    glfwSetFramebufferSizeCallback(this->window.ptr, this->framebufferSizeCallback);
    glfwSetInputMode(this->window.ptr, GLFW_STICKY_KEYS, 1);
    this->window.width = width;
    this->window.height = height;
}

void Env::setupController(void) const
{
    this->controller->setKeyProperties(GLFW_KEY_C, eKeyMode::toggle, 0, 1000);
}

void Env::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

anim_frames* Env::createWalkingAnimation(void)
{
    const auto walkingAnimation = new anim_frames({
        {
            // idle
            new std::vector<bone_transform>({
                {
                    {"upperArmLeft", vec3({0, 0, 0}), vec3({-0.7f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerArmLeft", vec3({0, 0, 0}), vec3({0.7f, 0, 0}), vec3({0, 0, 0})},
                    {"upperArmRight", vec3({0, 0, 0}), vec3({0.7f, 0.9f, -0.1f}), vec3({0, 0, 0})},
                    {"lowerArmRight", vec3({0, 0, 0}), vec3({0.5, 0, 0}), vec3({0, 0, 0})},
                    {"torso", vec3({0, 0, 0}), vec3({-0.2f, 0.1f, 0.025f}), vec3({0, 0, 0})},
                    {"pelvis", vec3({0, 0, 0}), vec3({0.2f, 0, -0.025f}), vec3({0, 0, 0})},
                    {"head", vec3({0, 0, 0}), vec3({0, 0, -0.05f}), vec3({0, 0, 0})},
                    {"upperLegLeft", vec3({0, 0, 0}), vec3({0.7f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegLeft", vec3({0, 0, 0}), vec3({-0.1f, 0, 0}), vec3({0, 0, 0})},
                    {"upperLegRight", vec3({0, 0, 0}), vec3({-0.4f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.2f, 0, 0}), vec3({0, 0, 0})},
                }
            }),
            // down
            new std::vector<bone_transform>({
                {
                    {"upperArmLeft", vec3({0, 0, 0}), vec3({-0.8f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerArmLeft", vec3({0, 0, 0}), vec3({0.6f, 0, 0}), vec3({0, 0, 0})},
                    {"upperArmRight", vec3({0, 0, 0}), vec3({0.8f, 0.5f, -0.05f}), vec3({0, 0, 0})},
                    {"lowerArmRight", vec3({0, 0, 0}), vec3({0.3f, 0, 0}), vec3({0, 0, 0})},
                    {"torso", vec3({0, 0, 0}), vec3({-0.2f, 0.2f, 0}), vec3({0, 0, 0})},
                    {"pelvis", vec3({0, 0, 0}), vec3({0.2f, 0, 0}), vec3({0, 0, 0})},
                    {"head", vec3({0, 0, 0}), vec3({0, 0, 0}), vec3({0, 0, 0})},
                    {"upperLegLeft", vec3({0, 0, 0}), vec3({0.8f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegLeft", vec3({0, 0, 0}), vec3({-0.8f, 0, 0}), vec3({0, 0, 0})},
                    {"upperLegRight", vec3({0, 0, 0}), vec3({-0.6f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.2f, 0, 0}), vec3({0, 0, 0})},
                }
            }),
            // avg
            new std::vector<bone_transform>({
                {
                    {"upperArmLeft", vec3({0, 0, 0}), vec3({-0.03f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerArmLeft", vec3({0, 0, 0}), vec3({0.25f, 0, 0}), vec3({0, 0, 0})},
                    {"upperArmRight", vec3({0, 0, 0}), vec3({0.3f, 0.2f, 0}), vec3({0, 0, 0})},
                    {"lowerArmRight", vec3({0, 0, 0}), vec3({0.3f, 0, 0}), vec3({0, 0, 0})},
                    {"torso", vec3({0, 0, 0}), vec3({-0.2f, 0.1f, -0.03f}), vec3({0, 0, 0})},
                    {"pelvis", vec3({0, 0, 0}), vec3({0.2f, 0, 0.07f}), vec3({0, 0, 0})},
                    {"head", vec3({0, 0, 0}), vec3({0, 0, 0.07f}), vec3({0, 0, 0})},
                    {"upperLegLeft", vec3({0, 0, 0}), vec3({-0.1f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegLeft", vec3({0, 0, 0}), vec3({0, 0, 0}), vec3({0, 0, 0})},
                    {"upperLegRight", vec3({0, 0, 0}), vec3({0.5, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegRight", vec3({0, 0, 0}), vec3({-1.8f, 0, 0}), vec3({0, 0, 0})},
                }
            }),
            // going up
            new std::vector<bone_transform>({
                {
                    {"upperArmLeft", vec3({0, 0, 0}), vec3({0.4f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerArmLeft", vec3({0, 0, 0}), vec3({0.2f, 0, 0}), vec3({0, 0, 0})},
                    {"upperArmRight", vec3({0, 0, 0}), vec3({-0.2f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerArmRight", vec3({0, 0, 0}), vec3({0.3f, 0, 0}), vec3({0, 0, 0})},
                    {"torso", vec3({0, 0, 0}), vec3({-0.2f, 0, -0.07f}), vec3({0, 0, 0})},
                    {"pelvis", vec3({0, 0, 0}), vec3({0.2f, 0, 0.13f}), vec3({0, 0, 0})},
                    {"head", vec3({0, 0, 0}), vec3({0, 0, 0.13f}), vec3({0, 0, 0})},
                    {"upperLegLeft", vec3({0, 0, 0}), vec3({-0.2f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegLeft", vec3({0, 0, 0}), vec3({-0.1f, 0, 0}), vec3({0, 0, 0})},
                    {"upperLegRight", vec3({0, 0, 0}), vec3({0.9f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegRight", vec3({0, 0, 0}), vec3({-1.2f, 0, 0}), vec3({0, 0, 0})},
                }
            }),
            // idle
            new std::vector<bone_transform>({
                {
                    {"upperArmLeft", vec3({0, 0, 0}), vec3({0.7f, -0.9f, 0.1f}), vec3({0, 0, 0})},
                    {"lowerArmLeft", vec3({0, 0, 0}), vec3({0.5, 0, 0}), vec3({0, 0, 0})},
                    {"upperArmRight", vec3({0, 0, 0}), vec3({-0.7f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerArmRight", vec3({0, 0, 0}), vec3({0.7f, 0, 0}), vec3({0, 0, 0})},
                    {"torso", vec3({0, 0, 0}), vec3({-0.2f, -0.1f, -0.025f}), vec3({0, 0, 0})},
                    {"pelvis", vec3({0, 0, 0}), vec3({0.2f, 0, 0.025f}), vec3({0, 0, 0})},
                    {"head", vec3({0, 0, 0}), vec3({0, 0, 0.05f}), vec3({0, 0, 0})},
                    {"upperLegLeft", vec3({0, 0, 0}), vec3({-0.4f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegLeft", vec3({0, 0, 0}), vec3({-0.2f, 0, 0}), vec3({0, 0, 0})},
                    {"upperLegRight", vec3({0, 0, 0}), vec3({0.7f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.1f, 0, 0}), vec3({0, 0, 0})},
                }
            }),
            // down
            new std::vector<bone_transform>({
                {
                    {"upperArmLeft", vec3({0, 0, 0}), vec3({0.8f, -0.5f, 0.05f}), vec3({0, 0, 0})},
                    {"lowerArmLeft", vec3({0, 0, 0}), vec3({0.6f, 0, 0}), vec3({0, 0, 0})},
                    {"upperArmRight", vec3({0, 0, 0}), vec3({-0.8f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerArmRight", vec3({0, 0, 0}), vec3({0.3f, 0, 0}), vec3({0, 0, 0})},
                    {"torso", vec3({0, 0, 0}), vec3({-0.2f, -0.2f, 0}), vec3({0, 0, 0})},
                    {"pelvis", vec3({0, 0, 0}), vec3({0.2f, 0, 0}), vec3({0, 0, 0})},
                    {"head", vec3({0, 0, 0}), vec3({0, 0, 0}), vec3({0, 0, 0})},
                    {"upperLegLeft", vec3({0, 0, 0}), vec3({-0.6f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegLeft", vec3({0, 0, 0}), vec3({-0.2f, 0, 0}), vec3({0, 0, 0})},
                    {"upperLegRight", vec3({0, 0, 0}), vec3({0.8f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.8f, 0, 0}), vec3({0, 0, 0})},
                }
            }),
            // avg
            new std::vector<bone_transform>({
                {
                    {"upperArmLeft", vec3({0, 0, 0}), vec3({0.3f, -0.2f, 0}), vec3({0, 0, 0})},
                    {"lowerArmLeft", vec3({0, 0, 0}), vec3({0.3f, 0, 0}), vec3({0, 0, 0})},
                    {"upperArmRight", vec3({0, 0, 0}), vec3({-0.03f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerArmRight", vec3({0, 0, 0}), vec3({0.25, 0, 0}), vec3({0, 0, 0})},
                    {"torso", vec3({0, 0, 0}), vec3({-0.2f, -0.1f, 0.03f}), vec3({0, 0, 0})},
                    {"pelvis", vec3({0, 0, 0}), vec3({0.2f, 0, -0.07f}), vec3({0, 0, 0})},
                    {"head", vec3({0, 0, 0}), vec3({0, 0, -0.07f}), vec3({0, 0, 0})},
                    {"upperLegLeft", vec3({0, 0, 0}), vec3({0.5, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegLeft", vec3({0, 0, 0}), vec3({-1.8f, 0, 0}), vec3({0, 0, 0})},
                    {"upperLegRight", vec3({0, 0, 0}), vec3({-0.1f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegRight", vec3({0, 0, 0}), vec3({0, 0, 0}), vec3({0, 0, 0})},
                }
            }),
            // up
            new std::vector<bone_transform>({
                {
                    {"upperArmLeft", vec3({0, 0, 0}), vec3({-0.2f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerArmLeft", vec3({0, 0, 0}), vec3({0.3f, 0, 0}), vec3({0, 0, 0})},
                    {"upperArmRight", vec3({0, 0, 0}), vec3({0.4f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerArmRight", vec3({0, 0, 0}), vec3({0.2f, 0, 0}), vec3({0, 0, 0})},
                    {"torso", vec3({0, 0, 0}), vec3({-0.2f, 0, 0.07f}), vec3({0, 0, 0})},
                    {"pelvis", vec3({0, 0, 0}), vec3({0.2f, 0, -0.13f}), vec3({0, 0, 0})},
                    {"head", vec3({0, 0, 0}), vec3({0, 0, -0.13f}), vec3({0, 0, 0})},
                    {"upperLegLeft", vec3({0, 0, 0}), vec3({0.9f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegLeft", vec3({0, 0, 0}), vec3({-1.2f, 0, 0}), vec3({0, 0, 0})},
                    {"upperLegRight", vec3({0, 0, 0}), vec3({-0.2f, 0, 0}), vec3({0, 0, 0})},
                    {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.1f, 0, 0}), vec3({0, 0, 0})},
                }
            }),

        }
    });
    return (walkingAnimation);
}

std::unordered_map<std::string, Bone*> Env::createCharacterSkeleton(void)
{
    std::unordered_map<std::string, Bone*> bones;
    bones["head"] = new Bone(
        std::forward_list<Bone*>{{}},
        "head",
        vec3({0, 0.425f, 0}),
        vec3({0, 0, 0}),
        vec3({0.8f, 0.85f, 0.8f}),
        vec3({0, -0.225f, 0}),
        0xEEAD7E
    );
    bones["neck"] = new Bone(
        std::forward_list<Bone*>{bones["head"]},
        "neck",
        vec3({0, 0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.3f, 0.3f, 0.3f}),
        vec3({0, 0, 0}),
        0xEEAD7E
    );
    bones["lowerLegLeft"] = new Bone(
        std::forward_list<Bone*>{{}},
        "lowerLegLeft",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.4f, 1, 0.4f}),
        vec3({0, 0.5, 0}),
        0x3F5D6A
    );
    bones["lowerLegRight"] = new Bone(
        std::forward_list<Bone*>{{}},
        "lowerLegRight",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.4f, 1, 0.4f}),
        vec3({0, 0.5, 0}),
        0x3F5D6A
    );
    bones["kneeLeft"] = new Bone(
        std::forward_list<Bone*>{bones["lowerLegLeft"]},
        "kneeLeft",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.3f, 0.3f, 0.3f}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["kneeRight"] = new Bone(
        std::forward_list<Bone*>{bones["lowerLegRight"]},
        "kneeRight",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.3f, 0.3f, 0.3f}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["upperLegLeft"] = new Bone(
        std::forward_list<Bone*>{bones["kneeLeft"]},
        "upperLegLeft",
        vec3({0, -0.5f, 0}),
        vec3({0, 0, 0}),
        vec3({0.5f, 1, 0.5f}),
        vec3({0, 0.5f, 0}),
        0x3F5D6A
    );
    bones["upperLegRight"] = new Bone(
        std::forward_list<Bone*>{bones["kneeRight"]},
        "upperLegRight",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.5, 1, 0.5}),
        vec3({0, 0.5, 0}),
        0x3F5D6A
    );
    bones["hipLeft"] = new Bone(
        std::forward_list<Bone*>{bones["upperLegLeft"]},
        "hipLeft",
        vec3({-0.3f, -0.4f, 0}),
        vec3({0, 0, 0}),
        vec3({0.3f, 0.3f, 0.3f}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["hipRight"] = new Bone(
        std::forward_list<Bone*>{bones["upperLegRight"]},
        "hipRight",
        vec3({0.3f, -0.4f, 0}),
        vec3({0, 0, 0}),
        vec3({0.3f, 0.3f, 0.3f}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["lowerArmLeft"] = new Bone(
        std::forward_list<Bone*>{{}},
        "lowerArmLeft",
        vec3({0, -0.4f, 0}),
        vec3({0, 0, 0}),
        vec3({0.35f, 0.8f, 0.35f}),
        vec3({0, 0.4f, 0}),
        0xEEAD7E
    );
    bones["lowerArmRight"] = new Bone(
        std::forward_list<Bone*>{{}},
        "lowerArmRight",
        vec3({0, -0.4f, 0}),
        vec3({0, 0, 0}),
        vec3({0.35f, 0.8f, 0.35f}),
        vec3({0, 0.4f, 0}),
        0xEEAD7E
    );
    bones["elbowLeft"] = new Bone(
        std::forward_list<Bone*>{bones["lowerArmLeft"]},
        "elbowLeft",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.3f, 0.3f, 0.3f}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["elbowRight"] = new Bone(
        std::forward_list<Bone*>{bones["lowerArmRight"]},
        "elbowRight",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.3f, 0.3f, 0.3f}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["upperArmLeft"] = new Bone(
        std::forward_list<Bone*>{bones["elbowLeft"]},
        "upperArmLeft",
        vec3({0, -0.45f, 0}),
        vec3({0, 0, 0}),
        vec3({0.4f, 0.9f, 0.4f}),
        vec3({0, 0.45f, 0}),
        0x408467
    );
    bones["upperArmRight"] = new Bone(
        std::forward_list<Bone*>{bones["elbowRight"]},
        "upperArmRight",
        vec3({0, -0.45f, 0}),
        vec3({0, 0, 0}),
        vec3({0.4f, 0.9f, 0.4f}),
        vec3({0, 0.45f, 0}),
        0x408467
    );
    bones["shoulderLeft"] = new Bone(
        std::forward_list<Bone*>{bones["upperArmLeft"]},
        "shoulderLeft",
        vec3({-0.85f, 0, 0}),
        vec3({0, 0, 0}),
        vec3({0.3f, 0.3f, 0.3f}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["shoulderRight"] = new Bone(
        std::forward_list<Bone*>{bones["upperArmRight"]},
        "shoulderRight",
        vec3({0.85f, 0, 0}),
        vec3({0, 0, 0}),
        vec3({0.3f, 0.3f, 0.3f}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["pelvis"] = new Bone(
        std::forward_list<Bone*>{{bones["hipLeft"], bones["hipRight"]}},
        "pelvis",
        vec3({0, -0.425f, 0}),
        vec3({0, 0, 0}),
        vec3({0.95f, 0.95f, 0.8f}),
        vec3({0, 0, 0}),
        0x43876A
    );
    bones["stomach"] = new Bone(
        std::forward_list<Bone*>{bones["pelvis"]},
        "stomach",
        vec3({0, -0.5f, 0}),
        vec3({0, 0, 0}),
        vec3({0.8f, 0.8f, 0.9f}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["torso"] = new Bone(
        std::forward_list<Bone*>{{bones["neck"], bones["stomach"], bones["shoulderLeft"], bones["shoulderRight"]}},
        "torso",
        vec3({0, 0.25f, 0}),
        vec3({0, 0, 0}),
        vec3({1.5f, 1, 1}),
        vec3({0, 0, 0}),
        0x43876A
    );
    return (bones);
}
