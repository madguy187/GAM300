#pragma once
//#include "ECS/ComponentManager/ComponentList.h"

#define M_PI       3.14159265358979323846   // pi
#define EPSILON 0.000001f
#define PTR_ADD( PTR, OFFSET ) \
  ((void *)(((char *)(PTR)) + (OFFSET)))
#define MAX_LAYER_SIZE 20

// Entities
using Entity = uint32_t;
static constexpr Entity MAX_ENTITY = 10000;

// Components and Signature
using ComponentType = std::uint8_t;
static constexpr ComponentType MAX_COMPONENT = 32;
using Signature = std::bitset<MAX_COMPONENT>;
static constexpr unsigned MAX_PAGE_SIZE = 10;

static const char* SCENE_EXTENSION = ".scn";
static const char* TEMP_PATH = "Data\\Temp\\";
static const char* ASSETS_PATH = "src\\Assets\\";
using EUUID = std::uint64_t;

enum class EditorWindowType
{
    EWT_HIERARCHY,
    EWT_INSPECTOR,
    EWT_SCENE,
    EWT_ASSETS,
    EWT_PROFILER,
    EWT_CONSOLELOG,
    EWT_GAMEVIEW,
    EWT_DEBUG,
    EWT_LOGGER,
    EWT_ASSETBROWSER,
    EWT_SWITCHVIEWS,
    EWT_SWITCHVIEW_TOP,
    EWT_SWITCHVIEW_BOTTOM,
    EWT_SWITCHVIEW_LEFT,
    EWT_SWITCHVIEW_RIGHT,
    EWT_HEADER,
    EWT_NODEEDITOR,
    EWT_MESHEDITOR,
    EWT_MATERIALEDITOR,
    EWT_UNASSIGNED
};

enum class EditorMenuType
{
    FILE,
    EDIT,
    WINDOWS,
    STYLE,
    ABOUTUS,
    UNASSIGNED
};

enum class ActorType
{
    ACTOR_UNASSIGNED,
    ACTOR_DYNAMIC,
    ACTOR_STATIC
};

enum class PxShapeType
{
    Px_SPHERE,
    Px_CUBE,
    Px_CAPSULE
};

enum class NodeType
{
    NODE_UNASSIGNED,
    NODE_ROOT,
    NODE_SELECTOR,
    NODE_SEQUENCE,
    NODE_BOOL,
    NODE_TASK
};

enum class EntityType
{
    // START OF GEO
    ENT_GEO_SQUARE, // FIRST
    //ENT_GEO_CIRCLE,
    //ENT_GEO_TRIANGLE,
    ENT_GEO_STAIRS,
    ENT_GEO_SPHERE,
    ENT_GEO_CUBE,
    ENT_GEO_CYLINDER,
    ENT_GEO_CONE,
    ENT_GEO_TORUS,
    //ENT_GEO_PYRAMID,
    ENT_GEO_ICOSPHERE,
    ENT_GEO_PLANES, 
    ENT_GEO_CURVE_STAIRS,
    ENT_GEO_SPIRAL_STAIRS,// LAST
    // END OF GEO
    // START OF LIGHTS
    ENT_LIGHT_POINT, // FIRST
    ENT_LIGHT_DIRECTIONAL,
    ENT_LIGHT_SPOT, // LAST
    // END OF LIGHTS
    ENT_GAMECAMERA,
    ENT_MODEL,
    ENT_TARGETPOINT,
    ENT_MESH,
    ENT_UNASSIGNED
};

enum class TypeToHighlight
{
    UNASSIGNED = 0,
    MODEL_3D = 1,
    MODE_BASIC = 2,
    MAXCOUNT
};
enum class PayloadSourceType
{
    PST_TEXT,
    PST_IMAGE,
    PST_ENTITY,
    PST_UNASSIGNED
};

enum class PayloadTargetType
{
	PTT_WIDGET,
	PTT_WINDOW,
	PTT_INDEXEDIT,
    PTT_ASSETS,
    PTT_SCRIPT,
	PTT_UNASSIGNED
};

enum class PayloadIndexJob
{
    PIJ_MOVE,
    PIJ_SWAP,
    PIJ_PARENTCHILD,
    PIJ_UNASSIGNED
};

enum class ModelType
{
    MT_UNASSIGNED = 0,
    MT_HUMAN = 1,
    MT_ANIMAL = 2,
    MT_HOUSE = 3,
    MT_ENVIRONMENT = 4,
    MT_MAXCOUNT
};

enum class TextureType
{
    TT_UNASSIGNED = 0,
    TT_2D = 1,
    TT_3D = 2,
    MT_MAXCOUNT
};

// Please keep this
enum class LoadingModels : unsigned int
{
    Square = 0,
    Triangle = 1,
    Circle = 2,
    Lines = 3,
    Lightsquare = 4,
    Sphere = 5,
    Plane = 6,
    Cube = 7,
    Cylinder = 8,
    Cone = 9,
    Torus = 10,
    Pyramid = 11,
    Lines3D = 12,
    MAXCOUNT
};

enum class EditComponent
{
    EC_ADDCOMPONENT,
    EC_REMOVECOMPONENT
};

enum class MaterialModelType
{
    MT_NONE = 0,
    MT_BASIC = 1,
    MT_MODELS3D = 2,
    MT_MAXCOUNT
};

enum class FrameBufferMode
{
    FBM_NONE = 0,

    // BASIC FRAMEBUFFERS
    FBM_GAME = 1,
    FBM_SCENE = 2,
    FBM_TOP = 3,
    FBM_BOTTOM = 4,
    FBM_LEFT = 5,
    FBM_RIGHT = 6,
    FBM_MATERIALEDITOR = 7,
    FBM_MESHEDITOR = 8,

    // SOBEL FBO
    FBM_GAME_SOBEL = 9,
    FBM_SCENE_SOBEL = 10,

    MAXCOUNT
};

enum class RenderMode
{
    Fill_Mode,
    Wireframe_Mode
};

enum class AudioType
{
    AT_SFX,
    AT_BGM,
    AT_EVENT,
    AT_UNASSIGNED
};

enum class m_Type
{
    MONO_UNDEFINED,
    MONO_VAR,
    MONO_HEADER,
    MONO_LAYERMASK,
    MONO_GAMEOBJECT,
    MONO_AUDIO,
    MONO_LIGHT,
    MONO_FLOAT
};

enum class ForceMode
{
    Default = 0,
    Impulse = 1
};

enum class CursorLockMode
{
    None = 0,
    Locked = 1,
    Confined = 2
};

struct MonoVariable
{
    m_Type type = m_Type::MONO_UNDEFINED;
    std::string varName = "";
    std::string varValue = "";
};

enum class TestState
{
    //TS_STOP = -1,
    TS_WALK,
    TS_RUN,
    TS_JUMP
};