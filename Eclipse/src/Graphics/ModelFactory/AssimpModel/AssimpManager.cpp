#include "pch.h"
#include "AssimpManager.h"

namespace Eclipse
{
	void AssimpModelManager::Init()
	{
		LoadModels("src//Assets//ASSModels");
		PrintLoadedModels();
	}

	void AssimpModelManager::LoadModels(const std::string& modelFile)
	{
		for (auto& dirEntry : std::filesystem::directory_iterator(modelFile))
		{
			const auto& path = dirEntry.path();
			auto relativePath = relative(path, "src//");
			std::string FolderName = relativePath.filename().string();

			std::string GoIntoModelFolder = ("src/Assets/ASSModels/" + FolderName);

			for (auto& dirEntry : std::filesystem::directory_iterator(GoIntoModelFolder))
			{
				const auto& FbxOrGltf = dirEntry.path();
				auto relativePath = relative(FbxOrGltf, "src//");
				std::string FbxOrGltfName = relativePath.filename().string();

				if ( FbxOrGltfName.find("gltf") != std::string::npos || FbxOrGltfName.find("fbx") != std::string::npos)
				{
					std::string PathName = ("src/Assets/ASSModels/" + FolderName + "/" + FbxOrGltfName).c_str();
					std::cout << "found!" << '\n';

					std::unique_ptr<AssimpModel> ptr(new AssimpModel(false));
					ptr->SetProperties(FolderName, ModelType::MT_ANIMAL);
					ptr->LoadAssimpModel(PathName);

					AssimpLoadedModels.emplace(FolderName, std::move(ptr));
					ModelMap.emplace(FolderName, PathName);
				}
			}
		}

		int i = 0;
		// First file path so i shd be src/assets/assmodels
		//for (auto& dirEntry : std::filesystem::directory_iterator("src//Assets//ASSModels"))
		//{
		//    const auto& path = dirEntry.path();
		//    auto relativePath = relative(path, "src//");
		//    std::string FolderName = relativePath.filename().string(); // Return me the name of the folder of my model
		//    std::string PathName = ("src/Assets/ASSModels/" + FolderName + "/").c_str();

		//    for (auto& dirEntry : std::filesystem::directory_iterator(PathName))
		//    {
		//        const auto& path = dirEntry.path();
		//        auto relativePath = relative(path, "src//");
		//        std::string NextFolderName = relativePath.filename().string();

		//        if (NextFolderName == "textures")
		//        {
		//            std::string InsideModelFolder = ("src/Assets/ASSModels/" + FolderName).c_str();
		//            std::string InsideTextures = ("src/Assets/ASSModels/" + FolderName + "/" + NextFolderName).c_str();

		//            unsigned int Index = 0;

		//            std::unique_ptr<Texture> LastTracked;

		//            std::string TexturePath;

		//            for (auto& dirEntry : std::filesystem::directory_iterator(InsideTextures))
		//            {
		//                const auto& path = dirEntry.path();
		//                auto relativePath = relative(path, "src//");
		//                std::string TextureName = relativePath.filename().string();

		//                 TexturePath = NextFolderName + "/" + TextureName;
		//                // If i find the first base colour , i insert.
		//                if (TextureName.find("baseColor") != std::string::npos)
		//                {
		//                    Texture tex(InsideModelFolder, TexturePath, aiTextureType_DIFFUSE);
		//                    std::unique_ptr<Texture> ptr(new Texture(tex));
		//                    ptr->Load(false);
		//                    engine->AssimpManager.InsertTextures(FolderName, std::move(ptr), Index);

		//                    std::unique_ptr<Texture> ptr2(new Texture(tex));
		//                    engine->AssimpManager.InsertTextures(FolderName, std::move(ptr2), Index);

		//                    LastTracked = std::unique_ptr<Texture>((new Texture(tex)));

		//                    Index++;
		//                }
		//            }

		//            if (AssimpLoadedModels[FolderName]->GetMesh().size() != Index)
		//            {
		//                Texture tex(InsideModelFolder, TexturePath, aiTextureType_DIFFUSE);
		//                std::unique_ptr<Texture> ptr(new Texture(tex));
		//                ptr->Load(false);
		//                engine->AssimpManager.InsertTextures(FolderName, std::move(ptr), Index);

		//                std::unique_ptr<Texture> ptr2(new Texture(tex));
		//                engine->AssimpManager.InsertTextures(FolderName, std::move(ptr2), Index);
		//            }
		//        }
		//    }
		//}

		ENGINE_CORE_INFO("All Assimp Models Loaded Once");
	}

	std::string AssimpModelManager::GetKey(const std::string& in)
	{
		return AssimpLoadedModels[in]->GetName();
	}

	void AssimpModelManager::CreateModel(unsigned int ID, std::string name, std::string FolderName, std::string filename)
	{
		// Add Required Components
		engine->world.AddComponent(ID, MaterialComponent{});

		TransformComponent& Transform = engine->world.GetComponent<TransformComponent>(ID);
		Transform.scale.setX(5);
		Transform.scale.setY(5);
		Transform.scale.setZ(5);

		MaterialComponent& mat = engine->world.GetComponent<MaterialComponent>(ID);
		mat.Modeltype = MaterialComponent::ModelType::Models3D;
		// ----------------------------------------------------------------------------------------------------------

		// Create path
		std::string PathName = ("src/Assets/ASSModels/" + FolderName + "/" + filename).c_str();

		// Test Path
		TestPath(PathName);

		// Always set False because we have textures
		AssimpModel* NewModel = new AssimpModel(false);
		NewModel->LoadAssimpModel(PathName);
		NewModel->SetProperties(name, ModelType::MT_ANIMAL, ID);

		// ----------------------------------------------------------------------------------------------------------

		//// Insert
		if (AssimpModelContainer_.insert(std::pair<unsigned int, AssimpModel*>(ID, NewModel)).second == true)
		{
			std::string Success = ("3D Model [" + name + "] Created and Inseted into Container Successfully! ").c_str();
			ENGINE_CORE_INFO(Success);
		}

		// ----------------------------------------------------------------------------------------------------------
		engine->MaterialManager.RegisterMeshForHighlighting(ID);
		// ----------------------------------------------------------------------------------------------------------
	}

	void AssimpModelManager::MeshDraw(unsigned int FrameBufferID, FrameBuffer::RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType)
	{
		auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(_camType));

		glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
		auto shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];

		shdrpgm.Use();

		for (auto const& Models : AssimpModelContainerV2)
		{
			auto& ID = Models.first;
			MeshComponent3D& ModelMesh = *(Models.second);

			engine->MaterialManager.UpdateStencilWithActualObject(ID);

			// Check Main Uniforms For each Model
			// Translation done here for each model
			CheckUniformLoc(shdrpgm, _camera, FrameBufferID, ID, box);

			if (_renderMode == FrameBuffer::RenderMode::Fill_Mode)
			{
				// Render
				Render(shdrpgm, GL_FILL, FrameBufferID, ModelMesh, ID);
			}
			else
			{
				Render(shdrpgm, GL_LINE, FrameBufferID, ModelMesh, ID);
			}

		}

		shdrpgm.UnUse();
	}

	void AssimpModelManager::Draw(unsigned int FrameBufferID, FrameBuffer::RenderMode _renderMode, AABB_* box, CameraComponent::CameraType _camType)
	{
		auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(_camType));

		glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
		auto shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];

		shdrpgm.Use();

		for (auto const& Models : AssimpModelContainer_)
		{
			auto& ID = Models.first;
			auto& InvidualModels = *(Models.second);

			engine->MaterialManager.UpdateStencilWithActualObject(ID);

			// Check Main Uniforms For each Model
			// Translation done here for each model
			CheckUniformLoc(shdrpgm, _camera, FrameBufferID, ID, box);

			if (_renderMode == FrameBuffer::RenderMode::Fill_Mode)
			{
				// Render
				InvidualModels.Render(shdrpgm, GL_FILL, FrameBufferID, ID);
			}
			else
			{
				InvidualModels.Render(shdrpgm, GL_LINE, FrameBufferID, ID);
			}

		}

		shdrpgm.UnUse();
	}

	void AssimpModelManager::HighlihtDraw(unsigned int FrameBufferID, GLenum Mode)
	{
		if (0) // if highlight is true
		{
			auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

			glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
			auto shdrpgm = Graphics::shaderpgms.find("OutLineShader");

			shdrpgm->second.Use();

			for (auto const& Models : AssimpModelContainer_)
			{
				auto& ID = Models.first;
				auto& InvidualModels = *(Models.second);

				// Check Main Uniforms For each Model
				// Translation done here for each model
				//CheckUniformLoc(shdrpgm->second, _camera, FrameBufferID, ID);

				GLuint uniformloc1 = shdrpgm->second.GetLocation("outlining");
				GLCall(glUniform1f(uniformloc1, 0.05f));

				// Render
				InvidualModels.Render(shdrpgm->second, Mode, FrameBufferID, ID);
			}

			shdrpgm->second.UnUse();
		}
	}

	void AssimpModelManager::CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int FrameBufferID, unsigned int ModelID, AABB_* box)
	{
		MaterialComponent& material = engine->world.GetComponent<MaterialComponent>(ModelID);
		GLint uniform_var_loc1 = _shdrpgm.GetLocation("material.shininess");
		GLint uniform_var_loc2 = _shdrpgm.GetLocation("material.MaximumShininess");

		GLCall(glUniform1f(uniform_var_loc1, material.shininess));
		GLCall(glUniform1f(uniform_var_loc2, material.MaximumShininess));


		TransformComponent& Transform = engine->world.GetComponent<TransformComponent>(ModelID);
		GLint uModelToNDC_ = _shdrpgm.GetLocation("uModelToNDC");
		GLuint model_ = _shdrpgm.GetLocation("model");
		GLuint dsa = _shdrpgm.GetLocation("noTex");

		if (uModelToNDC_ >= 0)
		{
			glm::mat4 mModelNDC;
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, Transform.position.ConvertToGlmVec3Type());
			model = glm::rotate(model, glm::radians(Transform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(Transform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(Transform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, Transform.scale.ConvertToGlmVec3Type());

			BoundingRegion br(Transform.position.ConvertToGlmVec3Type(), Transform.scale.ConvertToGlmVec3Type());
			box->AddInstance(br);

			mModelNDC = _camera.projMtx * _camera.viewMtx * model;
			glUniformMatrix4fv(uModelToNDC_, 1, GL_FALSE, glm::value_ptr(mModelNDC));
			glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
		}

		glUniform1i(dsa, 0);
	}

	MeshModelContainer AssimpModelManager::GetMeshContainer()
	{
		return AssimpModelContainerV2;
	}

	unsigned int AssimpModelManager::MeshModelCount()
	{
		return AssimpModelContainerV2.size();
	}

	unsigned int AssimpModelManager::MeshFactoryCount()
	{
		return AssimpLoadedModels.size();
	}

	AssimpModelContainer AssimpModelManager::GetContainer()
	{
		return AssimpModelContainer_;
	}

	unsigned int AssimpModelManager::AssimpModelCount()
	{
		return AssimpModelContainer_.size();
	}

	void AssimpModelManager::DeleteItem(unsigned int index, AssimpModel* model_ptr)
	{
		for (auto AssimpIT = AssimpModelContainer_.begin(); AssimpIT != std::end(AssimpModelContainer_); ++AssimpIT)
		{
			if (((*AssimpIT).first == index) && ((*AssimpIT).second == model_ptr))
			{
				AssimpModelContainer_.erase(AssimpIT);

				std::string Name = (*AssimpIT).second->GetName();
				std::string SuccessMsg = ("Model [ " + Name + " ] + erased from AssimpModelContainer_").c_str();
				ENGINE_CORE_INFO(SuccessMsg);
				return;
			}
		}
	}

	void AssimpModelManager::SetTexturesForModel(TextureComponent& in, std::string& passkey)
	{
		in.TextureKey = passkey;

		for (int i = 0; i <= LoadedTexturesV2[passkey][i].size(); i++)
		{
			if (LoadedTexturesV2.find(passkey) != LoadedTexturesV2.end())
			{
				if (LoadedTexturesV2[passkey][i].size() == 0)
				{
					EDITOR_LOG_WARN("No Textures Found for Current Model");
					continue;
				}

				for (int EachTexture = 0; EachTexture < LoadedTexturesV2[passkey][i].size(); EachTexture++)
				{
					in.HoldingTextures[i].push_back(*LoadedTexturesV2[passkey][i][EachTexture]);
				}
			}
		}
	}

	void AssimpModelManager::InsertTextures(std::string& NameofModel, std::unique_ptr<Texture> in, unsigned int MeshId)
	{
		LoadedTexturesV2[NameofModel][MeshId].push_back(std::move(in));
	}

	void AssimpModelManager::DeleteItem(unsigned int index)
	{
		AssimpMeshIT it = AssimpModelContainerV2.find(index);
		AssimpModelContainerV2.erase(index);
	}

	void AssimpModelManager::PrintLoadedModels()
	{
		std::cout << std::endl;
		std::cout << "Loaded Models Count " << AssimpLoadedModels.size() << std::endl;
		std::cout << "-------------------------------------------------------------------" << std::endl;

		for (auto const& Models : AssimpLoadedModels)
		{
			auto& InvidualModels = *(Models.second);
			std::cout << " Model Name : " << InvidualModels.GetName() << std::endl;
			std::cout << " Model Directory : " << InvidualModels.GetDirectory() << std::endl;
			std::cout << " Number of Textures : " << InvidualModels.GetNumberOfTextures() << std::endl;
			InvidualModels.GetTextureNames();
			std::cout << std::endl;
		}

		std::cout << "-------------------------------------------------------------------" << std::endl;

		std::cout << std::endl;
		std::cout << "Loaded Textures Count " << LoadedTexturesV2.size() << std::endl;
		std::cout << "-------------------------------------------------------------------" << std::endl;

		for (auto const& Model : LoadedTexturesV2)
		{
			auto& ModelName = (Model.first);
			auto& MapWithMeshIndexAndTextures = (Model.second);

			std::cout << "Model Name : " << ModelName << std::endl;

			for (auto& i : MapWithMeshIndexAndTextures)
			{
				for (auto& EachTextures : i.second)
				{
					std::cout << "MeshIndex [" << i.first << "] " << EachTextures->GetPath() << std::endl;
				}
			}
			std::cout << std::endl;
		}

		std::cout << "-------------------------------------------------------------------" << std::endl;
		std::cout << std::endl;
	}

	void AssimpModelManager::CleanUpAllModels()
	{
		for (auto const& Models : AssimpModelContainer_)
		{
			auto& InvidualModels = *(Models.second);
			InvidualModels.Cleanup();
		}
	}

	void AssimpModelManager::CleanUpAllModelsMeshes()
	{
		for (auto const& Models : AssimpModelContainerV2)
		{
			auto& InvidualModels = *(Models.second);
			Cleanup(InvidualModels);
		}
	}

	void AssimpModelManager::Cleanup(MeshComponent3D& in)
	{
		for (unsigned int i = 0; i < in.Meshes.size(); i++)
		{
			in.Meshes[i].Cleanup();
		}
	}

	AssimpModelManager::~AssimpModelManager()
	{
		//for (auto i : AssimpModelContainerV2)
		//{
		//    if (i.second != nullptr)
		//    {
		//        delete i.second;
		//    }
		//}
	}

	bool AssimpModelManager::InsertModel(AssimpModel& in)
	{
		// Insert
		if (AssimpModelContainer_.insert({ in.GetEntityID() , &in }).second == true)
		{
			return true;
		}

		return false;
	}

	bool AssimpModelManager::InsertMesh(MeshComponent3D& in)
	{
		// Insert
		if (AssimpModelContainerV2.insert({ in.ID , &in }).second == true)
		{
			return true;
		}

		return false;
	}

	bool AssimpModelManager::ClearContainer()
	{
		AssimpModelContainer_.clear();

		return true;
	}

	void AssimpModelManager::InsertModel(unsigned int id)
	{
		auto& sprite = engine->world.GetComponent<MeshComponent3D>(id);

		sprite.Directory = engine->AssimpManager.AssimpLoadedModels[sprite.Key]->GetDirectory();
		sprite.Meshes = engine->AssimpManager.AssimpLoadedModels[sprite.Key]->GetMesh();
		sprite.NameOfModel = engine->AssimpManager.AssimpLoadedModels[sprite.Key]->GetName();
		//sprite.NoTextures = engine->AssimpManager.AssimpLoadedModels[sprite.Key]->CheckNoTextures();
		sprite.Textures_loaded = engine->AssimpManager.AssimpLoadedModels[sprite.Key]->GetTextures();
		sprite.type = engine->AssimpManager.AssimpLoadedModels[sprite.Key]->GetType();

		if (AssimpModelContainerV2.insert({ sprite.ID ,&sprite }).second == true)
		{
			EDITOR_LOG_INFO(("Model : " + sprite.NameOfModel + "Created Successfully").c_str());
		}

		engine->MaterialManager.RegisterMeshForHighlight(sprite.ID);

		// If got TextureComponent
		if (engine->world.CheckComponent<TextureComponent>(id))
		{
			auto& tex = engine->world.GetComponent<TextureComponent>(id);
			engine->AssimpManager.SetTexturesForModel(tex, sprite.Key);
		}

		std::cout << "Current Model Container Size " << AssimpModelContainerV2.size() << std::endl;
	}

	void AssimpModelManager::TestPath(std::string& path)
	{
		std::ifstream test(path);

		if (!test)
		{
			std::string Error = ("The file path " + path + " doesnt exist! ").c_str();
			ENGINE_LOG_ASSERT(false, Error);
			std::exit(EXIT_FAILURE);
		}
	}

	AssimpModel* AssimpModelManager::GetModel(unsigned int ID)
	{
		for (auto const& it : AssimpModelContainer_)
		{
			auto& ID = (it.first);
			auto& SelectedModel = *(it.second);

			if (ID == ID)
			{
				return &SelectedModel;
			}
		}

		return nullptr;
	}

	void AssimpModelManager::Render(Shader& shader, GLenum MOde, unsigned int FrameBufferID, MeshComponent3D& in, unsigned int ModelID)
	{
		for (unsigned int i = 0; i < in.Meshes.size(); i++)
		{
			in.Meshes[i].Render(shader, MOde, ModelID, i);
		}
	}

	void AssimpModelManager::InsertModelMap(std::string& NameofModel, std::string& Directory)
	{
		ModelMap.insert({ NameofModel,Directory});
	}
}
