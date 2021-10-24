#ifndef ASSIMP_MANAGER_H
#define ASSIMP_MANAGER_H

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"
#include  <utility>

#include <vector>
#include "AssimpModel/AssimpModel.h"
#include "Graphics/Grid/AABBTree.h"

#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TextureComponent.h"
#include "ECS/ComponentManager/Components/ModelComponent.h"
#include "ECS/ComponentManager/Components/MaterialComponent.h"

#include "Graphics/Material/EnvironmentMap/EnvironmentMapManager.h"

namespace Eclipse
{
    class AssimpModelManager
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<Mesh>> Geometry;
        std::vector<std::string> AllPrimitiveModelsNames;
        std::unordered_map<std::string, std::vector<std::string>> Prefabs;
        std::vector<std::string> AllMeshNames;
        std::vector<std::string> AllGeometryNames;
        EnvironmentMapManager gEnvironmentMap;

    public:
        Entity CreateModel(unsigned int ID, const std::string& ModelName);
        void RegisterExistingModel(Entity ID, const std::string& ModelName);
        std::unordered_map<std::string, std::vector<std::string>>& GetPrefabs();
        void InsertPrimitiveName(const std::string& in);
        std::vector<std::string>& GetMeshNames();
        std::vector<std::string>& GetPrimitiveNames();
        void MeshDraw(MeshComponent& ModelMesh, unsigned int ID, FrameBufferMode in, RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType);
        void DebugNormals(MeshComponent& ModelMesh, unsigned int ID, FrameBufferMode in, CameraComponent::CameraType _camType);
        void CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int ModelID, AABB_* box);
        void Cleanup(MeshComponent& in);
        void TestPath(std::string& path);
        void Render(Shader& shader, GLenum MOde, MeshComponent&, unsigned int);
        ~AssimpModelManager();
        void SetMeshComponent(unsigned int ID, std::string);
        void Render(Shader& shader, GLenum mode, unsigned int id, MeshComponent& in);
        void Render(GLenum mode, MeshComponent& in);
        void SetSingleMesh(unsigned int ID, std::string& MeshName);
        AssimpModelManager() {};

        ///////////////////////////////////////////////////////////////////////////////////////////
        // For New PBR Shader
        ///////////////////////////////////////////////////////////////////////////////////////////
        void CheckUniforms(Shader&, Entity, MeshComponent&, CameraComponent&);
        void RenderMesh(MeshComponent& In, GLenum Mode);
        void ChecModelkUniforms(Shader& _shdrpgm, CameraComponent& _camera, unsigned int ModelID, AABB_* box);

        ///////////////////////////////////////////////////////////////////////////////////////////
        // For Compilers
        ///////////////////////////////////////////////////////////////////////////////////////////
        void ClearGeometry();
        void ClearAllMeshNames();
        void ClearAllPrefabs();
        void InsertGeometryName(const std::string& MeshName_);
        void InsertMeshName(const std::string& in);
        void InsertGeometry(const std::string& name, Mesh& NewMesh);
        void InsertPrefabs(const std::string& Index, const std::string& MeshName);

        // FOR FIKRUL
        void MeshEditorDraw(World& world_, MeshComponent& ModelMesh, unsigned int ID, FrameBufferMode in, CameraComponent::CameraType _camType);
        void MeshEditorUniforms(Shader& _shdrpgm, CameraComponent& _camera, unsigned int ModelID);
        
    public:
        // TEXTURES PUT HERE FIRST
        unsigned int Index = 0; // mesh index.
        void InsertTextures(std::string& NameofModel, std::unique_ptr<Texture> in, unsigned int MeshId);
        void SetTexturesForModel(MaterialComponent& in, std::string& passkey);
        bool GeometryContainerCheck(const std::string& in);
        bool CheckGeometryExist(MeshComponent& in);

    private:
        std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>> >LoadedTexturesV2;
    };
}
#endif // ASSIMP_MANAGER_H