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
#include "ECS/ComponentManager/Components/ModelInfoComponent.h"
#include "ECS/ComponentManager/Components/MaterialComponent.h"

namespace Eclipse
{
	typedef std::unordered_map<unsigned int, AssimpModel*> AssimpModelContainer;
	typedef std::unordered_map<unsigned int, MeshComponent*> MeshModelContainer;
	using AssimpIT = std::unordered_map<unsigned int, AssimpModel&>::iterator;
	using AssimpMeshIT = std::unordered_map<unsigned int, MeshComponent*>::iterator;
	using TextureIT = std::unordered_map<std::string, std::vector< std::unique_ptr<Texture>>>::iterator;

	class AssimpModelManager
	{
	private:
		bool HotReloadFlag = false;
		std::unordered_map<std::string, std::unique_ptr<Mesh>> Geometry;
		std::vector<std::string> AllPrimitiveModelsNames;
		std::unordered_map<std::string, std::vector<std::string>> Prefabs;
		std::vector<std::string> AllMeshNames;
	private:

	public:
		void ClearGeometry();
		void LoadBasicTextures();
		float HotReloadCooldown = 0.0f;
		bool GetHotReloadFlag();
		void ResetHotReloadFlag();
		bool CheckCompilers();
		void CreateModel(unsigned int ID, const std::string& ModelName);
		std::unordered_map<std::string, std::vector<std::string>>& GetPrefabs();
		void InsertPrimitiveName(const std::string& in);
		void InsertMeshName(const std::string& in);
		std::vector<std::string>& GetMeshNames();
		std::vector<std::string>& GetPrimitiveNames();
		// Get Current MeshComponent Container
		MeshModelContainer GetMeshContainer();
		// Ger how many Models in Container
		unsigned int MeshModelCount();
		// Get Current Model Factory Count
		unsigned int MeshFactoryCount();
		// Load All Models Once
		void Init();
		static void ExecuteCompiler();
		void HotReload();
		void HotReloadTetxures();
		// Render Function that uses the Container that stores MeshComponent 
		void MeshDraw(MeshComponent& ModelMesh , unsigned int ID, unsigned int FrameBufferID, FrameBuffer::RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType);
		// Upload to Shader
		void CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int FrameBufferID, unsigned int ModelID, AABB_* box);
		// Delete Model from MeshContainer Using ID
		void DeleteItem(unsigned int index);
		// Print out all the Loaded Model details
		void PrintLoadedModels();
		// Destory buffers
		void CleanUpAllModelsMeshes();
		// Cleanup
		void Cleanup(MeshComponent& in);
		// Check Current path is correct
		void TestPath(std::string& path);
		// Draw function that takes in Mesh Component
		void Render(Shader& shader, GLenum MOde, unsigned int FrameBufferID, MeshComponent& , unsigned int ,CameraComponent::CameraType );
		void InsertModelMap(std::string& NameofModel, std::string& Directory);
		// Model Factory to load all models
		void LoadCompilers();
		// Destructor
		~AssimpModelManager();
		void SetMeshComponent(unsigned int ID, std::string);
		void Render(Shader& shader, GLenum mode, unsigned int id, MeshComponent& in);
		void Render(GLenum mode, MeshComponent& in);
		void SetSingleMesh(unsigned int ID, std::string& MeshName);
		void LoadGeometry();
		void LoadPrefabs();
		void LoadTextures();
	    AssimpModelManager() {};

		///////////////////////////////////////////////////////////////////////////////////////////
		// FDebug PrintOuts
		///////////////////////////////////////////////////////////////////////////////////////////
		void PrintOutModelsLoadedOnce(); // Print Out the names of all the models loaded once
		void PrintOutModelTextureMap();  // Print out the names of the textures mapped to the MeshIndex of the model
		void PrintOutModelMap(); // Print out Model Name and Directory of the loaded Models
		void PrintOutAllTextures();
		void PrintOutAllMeshes();


	public:
		// TEXTURES PUT HERE FIRST
		unsigned int Index = 0; // mesh index.
		void InsertTextures(std::string& NameofModel, std::unique_ptr<Texture> in, unsigned int MeshId);
		void SetTexturesForModel(MaterialComponent& in, std::string& passkey);

	private:
		AssimpModelContainer AssimpModelContainer_;
		MeshModelContainer AssimpModelContainerV2;
		std::unordered_map<std::string, std::unique_ptr<AssimpModel>> AssimpLoadedModels;
		std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>> >LoadedTexturesV2;
		std::unordered_map<std::string, std::string> ModelMap;
		bool InsertMesh(MeshComponent& in);
		bool ClearContainer();
		void CleanUpAllModels();
		unsigned int AssimpModelCount();
		bool InsertModel(AssimpModel& in);
		AssimpModelContainer GetContainer();
		AssimpModel* GetModel(unsigned int ID);
		void CreateModel(unsigned int ID, std::string name, std::string path, std::string filename);
		void Draw(unsigned int FrameBufferID, FrameBuffer::RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType);
		void HighlihtDraw(unsigned int FrameBufferID, GLenum Mode);
		void DeleteItem(unsigned int index, AssimpModel* model_ptr);
		std::string GetKey(const std::string& in);
		void InsertModel(unsigned int ID);

	};
}
#endif // ASSIMP_MANAGER_H