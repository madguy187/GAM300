#ifndef ASSIMP_MANAGER_H
#define ASSIMP_MANAGER_H

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"
#include  <utility>

#include <vector>
#include "AssimpModel/AssimpModel.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "Graphics/Grid/AABBTree.h"
#include "ECS/ComponentManager/Components/TextureComponent.h"
namespace Eclipse
{
    typedef std::unordered_map<unsigned int, AssimpModel*> AssimpModelContainer;
    typedef std::unordered_map<unsigned int, MeshComponent3D*> MeshModelContainer;
    using AssimpIT = std::unordered_map<unsigned int, AssimpModel&>::iterator;
    using AssimpMeshIT = std::unordered_map<unsigned int, MeshComponent3D*>::iterator;
    using TextureIT = std::unordered_map<std::string, std::vector< std::unique_ptr<Texture>>>::iterator;
    // Current Version
    // Prev : Loading Model in every Create , Store them into Container as AssimpModel*
    // Now  : All Models are Only Loaded Once , Store them as Mesh* in AssimpLoadedModels.

    class AssimpModelManager
    {
    private:
        // Version 2 Container that stores AssimpModel*
        MeshModelContainer AssimpModelContainerV2;
        // Container to store Models who are loaded once
        std::unordered_map<std::string, std::unique_ptr<AssimpModel>> AssimpLoadedModels;

        // Version 1 Container that stores AssimpModel*
        AssimpModelContainer AssimpModelContainer_;

        // Name of Model , < MeshIndex, Texture Container > 
        std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>> >LoadedTexturesV2;
        std::multimap<std::string, std::unique_ptr<Texture>> LoadedTextures;
    public:
        // Get Current MeshComponent Container
        MeshModelContainer GetMeshContainer();
        // Ger how many Models in Container
        unsigned int MeshModelCount();
        // Get Current Model Factory Count
        unsigned int MeshFactoryCount();
        // Load All Models Once
        void LoadAllModels();
        // Render Function that uses the Container that stores MeshComponent 
        void MeshDraw(unsigned int FrameBufferID, FrameBuffer::RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType);
        // Upload to Shader
        void CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int FrameBufferID, unsigned int ModelID, AABB_* box);
        // Delete Model from MeshContainer Using ID
        void DeleteItem(unsigned int index);
        // Print out all the Loaded Model details
        void PrintLoadedModels();
        // Destory buffers
        void CleanUpAllModelsMeshes();
        // Cleanup
        void Cleanup(MeshComponent3D& in);
        // Check Current path is correct
        void TestPath(std::string& path);
        // Draw function that takes in Mesh Component
        void Render(Shader& shader, GLenum MOde, unsigned int FrameBufferID, MeshComponent3D& in, unsigned int inin);
        // Using MeshComponent into Container , Pass in key please
        void InsertModel(MeshComponent3D& in, std::string& key);
        // Model Factory to load all models
        void LoadModels(const std::string& modelFile);
        // Get Key
        std::string GetKey(const std::string& in);
        // Destructor
        ~AssimpModelManager();

        ///////////////////////////////////////////////////////////////////////////////////////////
        // For Jian Herng
        ///////////////////////////////////////////////////////////////////////////////////////////
        bool InsertMesh(MeshComponent3D& in);
        bool ClearContainer();

    private:
        //
        void CleanUpAllModels();
        unsigned int AssimpModelCount();
        bool InsertModel(AssimpModel& in);
        AssimpModelContainer GetContainer();
        AssimpModel* GetModel(unsigned int ID);
        void CreateModel(unsigned int ID, std::string name, std::string path, std::string filename);
        void Draw(unsigned int FrameBufferID, FrameBuffer::RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType);
        void HighlihtDraw(unsigned int FrameBufferID, GLenum Mode);
        void DeleteItem(unsigned int index, AssimpModel* model_ptr);

    public:
        // TEXTURES PUT HERE FIRST
        
        void SetTexturesForModel(TextureComponent& in, std::string& passinkey);
        void InsertTextures(std::string& NameofModel, std::unique_ptr<Texture> in, unsigned int MeshId);
    };
}
#endif // ASSIMP_MANAGER_H