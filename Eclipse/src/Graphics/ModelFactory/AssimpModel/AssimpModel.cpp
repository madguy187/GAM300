#include "pch.h"
#include "AssimpModel/AssimpModel.h"
#include <regex>
#include "Logger/Logger.h"

namespace Eclipse
{

	AssimpModel::AssimpModel(bool noTex)
		:
		noTex(noTex)
	{
		//GlobalMode = GL_FILL;
	}

	void AssimpModel::Render(Shader& shader, GLenum MOde, unsigned int FrameBufferID)
	{
		for (unsigned int i = 0; i < Meshes.size(); i++)
		{
			Meshes[i].Render(shader, MOde);
		}
	}

	void AssimpModel::Cleanup()
	{
		for (unsigned int i = 0; i < Meshes.size(); i++)
		{
			Meshes[i].Cleanup();
		}
	}

	void AssimpModel::LoadAssimpModel(std::string path)
	{
		unsigned int importOptions =
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_JoinIdenticalVertices |
			aiProcess_FlipWindingOrder |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_FindDegenerates |
			aiProcess_FindInvalidData |
			aiProcess_GenUVCoords |
			aiProcess_TransformUVCoords |
			aiProcess_FindInstances |
			aiProcess_PreTransformVertices;

		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, importOptions);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::string Error = ("Could not load AssimpModel at " + path + import.GetErrorString()).c_str();
			return;
		}

		Directory = path.substr(0, path.find_last_of("/"));
		//glm::mat4 Model = (glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
		//scene->mRootNode->mTransformation = aiMatrix4x4();

		ProcessNode(scene->mRootNode, scene);
	}

	void AssimpModel::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process all meshes
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			Meshes.push_back(ProcessMesh(mesh, scene));
		}

		// process all child nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			//node->mChildren[i]->mTransformation = scene->mRootNode->mTransformation * node->mChildren[i]->mTransformation;
			ProcessNode(node->mChildren[i], scene);
		}
	}

	float Eclipse::AssimpModel::GetLargestAxisValue(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
	{
		float minValue = (std::min)({ _minmaxX.first, _minmaxY.first, _minmaxZ.first });
		float maxValue = (std::max)({ _minmaxX.second, _minmaxY.second, _minmaxZ.second });

		float largestAxis = (std::max)(std::abs(minValue), maxValue);

		return largestAxis;
	}

	void Eclipse::AssimpModel::ComputeAxisMinMax(std::vector<glm::vec3>& vertices, std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
	{
		std::vector<float> verticesX;
		std::vector<float> verticesY;
		std::vector<float> verticesZ;

		for (auto& it : vertices)
		{
			verticesX.push_back(it.x);
			verticesY.push_back(it.y);
			verticesZ.push_back(it.z);
		}

		auto minmaxX = std::minmax_element(verticesX.begin(), verticesX.end());
		auto minmaxY = std::minmax_element(verticesY.begin(), verticesY.end());
		auto minmaxZ = std::minmax_element(verticesZ.begin(), verticesZ.end());

		_minmaxX.first = *minmaxX.first;
		_minmaxX.second = *minmaxX.second;

		_minmaxY.first = *minmaxY.first;
		_minmaxY.second = *minmaxY.second;

		_minmaxZ.first = *minmaxZ.first;
		_minmaxZ.second = *minmaxZ.second;
	}

	glm::vec3 Eclipse::AssimpModel::ComputeCentroid(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ)
	{
		glm::vec3 centroid = glm::vec3{
			(_minmaxX.first + _minmaxX.second) / 2.0f,
			(_minmaxY.first + _minmaxY.second) / 2.0f,
			(_minmaxZ.first + _minmaxZ.second) / 2.0f };

		return centroid;
	}

	void AssimpModel::SetName(std::string& name)
	{
		NameOfModel = name;
	}

	std::string AssimpModel::GetDirectory()
	{
		return Directory;
	}

	unsigned int Eclipse::AssimpModel::GetNumberOfTextures()
	{
		return Textures_loaded.size();
	}

	unsigned int Eclipse::AssimpModel::GetNumberOfMeshes()
	{
		return Meshes.size();
	}

	ModelType Eclipse::AssimpModel::GetType()
	{
		return type;
	}

	void Eclipse::AssimpModel::SetModelType(ModelType in)
	{
		type = in;
	}

	std::vector<glm::vec3> Eclipse::AssimpModel::GetVertices()
	{
		return AllVertices;
	}

	void Eclipse::AssimpModel::SetProperties(std::string& ModelName, ModelType in, unsigned int ID_)
	{
		ID = ID_;
		SetName(ModelName);
		SetModelType(in);
	}

	//DYN_AABB Eclipse::AssimpModel::getAABB() const
	//{
	//    return AABB_Property;
	//}
	//
	//DYN_AABB Eclipse::AssimpModel::SetAABB(TransformComponent& in)
	//{
	//    glm::vec3 scale = in.scale.ConvertToGlmVec3Type();
	//    glm::vec3 position = in.position.ConvertToGlmVec3Type();
	//
	//    glm::vec3 halfExt = scale / 2.0f;
	//    glm::vec3 min{ position.x - halfExt.x, position.y - halfExt.y, position.z - halfExt.z };
	//    glm::vec3 max{ position.x + halfExt.x, position.y + halfExt.y, position.z + halfExt.z };
	//
	//    AABB_Property.Min = min;
	//    AABB_Property.Max = max;
	//
	//    return AABB_Property;
	//}

	unsigned int Eclipse::AssimpModel::GetEntityID()
	{
		return ID;
	}

	void Eclipse::AssimpModel::GetTextureNames()
	{
		for (int i = 0; i < Textures_loaded.size(); i++)
		{
			std::cout << " Texture Name " << Textures_loaded[i].GetPath() << std::endl;
		}
	}

	std::string AssimpModel::GetName()
	{
		return NameOfModel;
	}

	Mesh AssimpModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		// vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			// position
			vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

			// normal vectors
			vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			// textures
			if (mesh->mTextureCoords[0])
			{
				vertex.TextureCoodinates = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertex.TextureCoodinates = glm::vec2(0.0f);
			}

			AllVertices.push_back(vertex.Position);
			vertices.push_back(vertex);
		}

		std::pair<float, float> minmaxX;
		std::pair<float, float> minmaxY;
		std::pair<float, float> minmaxZ;

		ComputeAxisMinMax(AllVertices, minmaxX, minmaxY, minmaxZ);

		glm::vec3 centroid = ComputeCentroid(minmaxX, minmaxY, minmaxZ);
		float largestAxis = GetLargestAxisValue(minmaxX, minmaxY, minmaxZ);

		for (auto& it : AllVertices)
		{
			it -= centroid;

			it.x /= largestAxis;
			it.y /= largestAxis;
			it.z /= largestAxis;
		}

		for (auto& it : vertices)
		{
			it.Position -= centroid;

			it.Position.x /= largestAxis;
			it.Position.y /= largestAxis;
			it.Position.z /= largestAxis;
		}

		// process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// process material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			aiString texture_file;
			material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);

			if (auto texture = scene->GetEmbeddedTexture(texture_file.C_Str()))
			{
				std::cout << "Embedded" << std::endl;
				// Not working for fbx file leh
			}
			else
			{
				//regular file, check if it exists and read it
				if (noTex)
				{
					// diffuse color
					aiColor4D diff(1.0f);
					aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);

					// specular color
					aiColor4D spec(1.0f);
					aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

					return Mesh(vertices, indices, diff, spec);
				}

				// diffuse maps
				std::vector<Texture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

				// specular maps
				std::vector<Texture> specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture> AssimpModel::LoadTextures(aiMaterial* mat, aiTextureType type)
	{
		std::vector<Texture> textures;

		// need my own reader for fbx textures
		//Texture tex(directory, "textures/default_Base_Color.png", type);
		//tex.load(false);
		//textures.push_back(tex);
		//textures_loaded.push_back(tex);

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			// prevent duplicate loading
			bool skip = false;
			for (unsigned int j = 0; j < Textures_loaded.size(); j++)
			{
				if (std::strcmp(Textures_loaded[j].GetPath().data(), str.C_Str()) == 0)
				{
					textures.push_back(Textures_loaded[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				// not loaded yet
				//std::cout << str.C_Str() << std::endl;

				Texture tex(Directory, str.C_Str(), type);
				tex.Load(false);
				textures.push_back(tex);
				Textures_loaded.push_back(tex);
			}
		}

		return textures;
	}


	void Eclipse::AssimpModel::initModel()
	{

	}

	void Eclipse::AssimpModel::DeleteModel()
	{

	}
}