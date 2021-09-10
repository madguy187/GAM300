#pragma once
#include "pch.h"

#define EPSILON 0.000001f

// Entities
using Entity = uint32_t;
static constexpr Entity MAX_ENTITY = 10000;

// Components and Signature
using ComponentType = std::uint8_t;
static constexpr ComponentType MAX_COMPONENT = 32;
using Signature = std::bitset<MAX_COMPONENT>;
static constexpr unsigned MAX_PAGE_SIZE = 10;

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
    SWITCHVIEW_TOP,
    SWITCHVIEW_Bottom,
    SWITCHVIEW_Left,
    SWITCHVIEW_Right,
    EWT_UNASSIGNED
};

enum class EditorMenuType
{
    FILE,
    EDIT,
    WINDOWS,
    HELP,
    UNASSIGNED
};

enum class EntityType
{
    ENT_GEO_SQUARE,
    ENT_GEO_CIRCLE,
    ENT_GEO_TRIANGLE,
    ENT_GEO_LINES,
    ENT_GEO_SPHERE,
    ENT_GEO_CUBE,
    ENT_GEO_CYLINDER,
    ENT_GEO_CONE,
    ENT_GEO_TORUS,
    ENT_GEO_PYRAMID,
    ENT_GEO_LINES3D,
    ENT_GEO_PLANES,
    ENT_LIGHT_POINT,
    ENT_LIGHT_DIRECTIONAL,
    ENT_LIGHT_SPOT,
    // ADD ON LIGHTS HERE
    ENT_CAMERA,
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
    PST_UNASSIGNED
};

enum class PayloadTargetType
{
	PTT_WIDGET,
	PTT_WINDOW,
	PTT_INDEXEDIT,
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
    square = 0,
    triangle = 1,
    circle = 2,
    lines = 3,
    lightsquare = 4,
    sphere = 5,
    plane = 6,
    cube = 7,
    cylinder = 8,
    cone = 9,
    torus = 10,
    pyramid = 11,
    lines3D = 12,
    MAXCOUNT
};

enum class EditComponent
{
    EC_ADDCOMPONENT,
    EC_REMOVECOMPONENT
};