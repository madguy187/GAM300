#ifndef ASSIMP_MANAGER_H
#define ASSIMP_MANAGER_H

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"

#include <vector>
#include "AssimpModel/AssimpModel.h"

using namespace Eclipse;

typedef std::map<unsigned int, AssimpModel*> AssimpModelContainer;
using AssimpIT = std::map<unsigned int, AssimpModel*>::iterator;

class AssimpModelManager
{
private:
    AssimpModelContainer AssimpModelContainer_;

public:
    AssimpModelContainer GetContainer();
    unsigned int AssimpModelCount();
    void LoadAllModels();
    void CreateModel(std::string name, std::string path);
    void Draw(unsigned int FrameBufferID, GLenum mode);
    void DeleteItem(unsigned int index, AssimpModel* model_ptr);
    void DebugPrint();
    void AddComponents();
    void CleanUpAllModels();
};
#endif // ASSIMP_MANAGER_H