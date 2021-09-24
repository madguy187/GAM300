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

		// Name of Model , < MeshIndex, Texture Container > 
		std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>> >LoadedTexturesV2;

		// Model Information
		std::unordered_map<std::string, std::string> ModelMap;
		
	public:
		// Single Meshes
		std::unordered_map<std::string, std::unique_ptr<Mesh>> SingleMeshMap;

		// Container to store Models who are loaded once
		std::unordered_map<std::string, std::unique_ptr<AssimpModel>> AssimpLoadedModels;

		// Container to store all mesh names
		std::vector<std::string> AllMeshNames;

		// Get Current MeshComponent Container
		MeshModelContainer GetMeshContainer();
		// Ger how many Models in Container
		unsigned int MeshModelCount();
		// Get Current Model Factory Count
		unsigned int MeshFactoryCount();
		// Load All Models Once
		void Init();
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
		// Using MeshComponent into Container , Pass in key please
		void InsertModel(unsigned int ID);
		// Model Factory to load all models
		void LoadModels(const std::string& modelFile);
		// Get Key
		std::string GetKey(const std::string& in);
		// Destructor
		~AssimpModelManager();
		void SetMeshComponent(unsigned int ID, std::string);
		void Render(Shader& shader, GLenum mode, unsigned int id, MeshComponent& in);
		void Render(GLenum mode, MeshComponent& in);
		void SetSingleMesh(unsigned int ID, std::string& MeshName);

		///////////////////////////////////////////////////////////////////////////////////////////
		// FDebug PrintOuts
		///////////////////////////////////////////////////////////////////////////////////////////
		void PrintOutModelsLoadedOnce(); // Print Out the names of all the models loaded once
		void PrintOutModelTextureMap();  // Print out the names of the textures mapped to the MeshIndex of the model
		void PrintOutModelMap(); // Print out Model Name and Directory of the loaded Models
		void PrintOutAllTextures();
		void PrintOutAllMeshes();


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
		unsigned int Index = 0; // mesh index.
		void InsertTextures(std::string& NameofModel, std::unique_ptr<Texture> in, unsigned int MeshId);
		void SetTexturesForModel(MaterialComponent& in, std::string& passkey);
	private:
		// Version 1 Container that stores AssimpModel*
		AssimpModelContainer AssimpModelContainer_;
		// Version 2 Container that stores AssimpModel*
		MeshModelContainer AssimpModelContainerV2;

		///////////////////////////////////////////////////////////////////////////////////////////
		// For Jian Herng
		///////////////////////////////////////////////////////////////////////////////////////////
		bool InsertMesh(MeshComponent& in);
		bool ClearContainer();

	};
}
#endif // ASSIMP_MANAGER_H