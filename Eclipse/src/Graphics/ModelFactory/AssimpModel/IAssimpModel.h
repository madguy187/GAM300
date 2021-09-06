#ifndef IMODELASSIMP_H
#define IMODELASSIMP_H

#include "glew.h"
#include "glm.hpp"
#include "Parser.h"
#include <string>
#include <vector>

class IAssimpModel
{
public:
    virtual ~IAssimpModel() = default;

    virtual void initModel() = 0;
    virtual void DeleteModel() = 0;
};

#endif /* IMODELASSIMP_H */
