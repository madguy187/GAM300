#ifndef ASSIMP_MANAGER_H
#define ASSIMP_MANAGER_H

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"

#include <vector>
#include "AssimpModel/AssimpModel.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "Graphics/Grid/AABBTree.h"
#include "Graphics/ModelFactory/AssimpModel/IAssimpModel.h"

namespace Eclipse
{
	typedef std::unordered_map<unsigned int, AssimpModel*> AssimpModelContainer;
	using AssimpIT = std::unordered_map<unsigned int, AssimpModel*>::iterator;

	class AssimpModelManager
	{
	private:
		// Main Container to iterate my models
		AssimpModelContainer AssimpModelContainer_;
		// Container to store Models who are loaded once
		std::unordered_map<std::string, std::unique_ptr<IAssimpModel>> AssimpLoadedModels;

	public:
		AssimpModelContainer GetContainer();
		unsigned int AssimpModelCount();
		void LoadAllModels();
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
		~AssimpModelManager();
		bool InsertModel(AssimpModel& in);
		bool ClearContainer();

		// Model Factory Factory 
		#ifndef MODEL_FACOTRUY
		static std::unique_ptr<IAssimpModel> CreateModel(unsigned int ModelIndex);
		void LoadModels(std::string& modelFile);
        #else
		#endif
	};
}
#endif // ASSIMP_MANAGER_H