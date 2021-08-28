#ifndef ASSIMP_MANAGER_H
#define ASSIMP_MANAGER_H

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"

#include <vector>
#include "AssimpModel/AssimpModel.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "Graphics/Grid/AABBTree.h"

namespace Eclipse
{
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
        void CreateModel(std::string name, std::string path, std::string filename);
        void Draw(unsigned int FrameBufferID, GLenum mode, AABB_* box, CameraComponent::CameraType _camType);
        void HighlihtDraw(unsigned int FrameBufferID, GLenum Mode);
        void CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int FrameBufferID, unsigned int ModelID, AABB_* box);
        void DeleteItem(unsigned int index, AssimpModel* model_ptr);
        void DebugPrint();
        void AddComponents();
        void CleanUpAllModels();
        void TestPath(std::string& path);
        AssimpModel* GetModel(unsigned int ID);
        ~AssimpModelManager();

        void addSpriteToCollisionSet(const std::shared_ptr<AssimpModel>& sprite);
    };
}
#endif // ASSIMP_MANAGER_H