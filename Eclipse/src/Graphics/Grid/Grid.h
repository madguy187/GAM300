#pragma once
#include "Graphics/ModelFactory/BasicPrimitives/Quad.h"

class Grid
{
public:
    unsigned int ID;
    Quad* Single;
    Shader* ShaderRef;
    IModel* modelRef;

    void init();
    void DrawGrid();
};