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

namespace Eclipse
{
	typedef std::unordered_map<unsigned int, AssimpModel*> AssimpModelContainer;
	typedef std::unordered_map<unsigned int, MeshComponent3D*> MeshModelContainer;
	using AssimpIT = std::unordered_map<unsigned int, AssimpModel&>::iterator;

	// Current Version
	// Prev : Loading Model in every Create , Store them into Container as AssimpModel*
	// Now  : All Models are Only Loaded Once , Store them as Mesh* in AssimpLoadedModels.

 	class AssimpModelManager
	{
	private:
		// Version 1 Container that stores AssimpModel*
		AssimpModelContainer AssimpModelContainer_;
		// Version 2 Container that stores AssimpModel*
		MeshModelContainer AssimpModelContainerV2;
		// Container to store Models who are loaded once
		std::unordered_map<std::string, std::unique_ptr<AssimpModel>> AssimpLoadedModels;

	public:

		MeshModelContainer GetMeshContainer();
		unsigned int MeshModelCount();
		void LoadAllModels();
		void MeshDraw(unsigned int FrameBufferID, GLenum Mode, AABB_* box, CameraComponent::CameraType _camType);
		void CreateModel(unsigned int ID, std::string name, std::string path, std::string filename);
		void Draw(unsigned int FrameBufferID, GLenum mode, AABB_* box, CameraComponent::CameraType _camType);
		void HighlihtDraw(unsigned int FrameBufferID, GLenum Mode);
		void CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int FrameBufferID, unsigned int ModelID, AABB_* box);
		void DeleteItem(unsigned int index, AssimpModel* model_ptr);
		void DebugPrint();
		void AddComponents();
		void CleanUpAllModels();
		void TestPath(std::string& path);
		AssimpModel* GetModel(unsigned int ID);
	    void Render(Shader& shader, GLenum MOde, unsigned int FrameBufferID, MeshComponent3D& in);
		~AssimpModelManager();
		bool InsertModel(AssimpModel& in);
		bool ClearContainer();

		// Using MeshComponent
		void InsertModel(MeshComponent3D& in , std::string& key);
		
		// Model Factory Factory 
		#ifndef MODEL_FACOTRUY
		void LoadModels(std::string& modelFile);
		std::string GetKey(const std::string& in);
        #else
		#endif

		AssimpModelContainer GetContainer();
		unsigned int AssimpModelCount();
	};
}
#endif // ASSIMP_MANAGER_H