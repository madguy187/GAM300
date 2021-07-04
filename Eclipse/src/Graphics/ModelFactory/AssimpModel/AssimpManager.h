#ifndef ASSIMP_MANAGER_H
#define ASSIMP_MANAGER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
    void LoadAllModels();
    void CreateModel(std::string name, std::string path);
    void DrawBuffers();
    AssimpModelContainer GetContainer();
    unsigned int AssimpModelCount();
    void DeleteItem(unsigned int index, AssimpModel* model_ptr);
    void DebugPrint();
    void AddComponents();
};
#endif // ASSIMP_MANAGER_H