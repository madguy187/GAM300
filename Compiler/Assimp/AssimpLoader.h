#pragma once
#include "BinarySettings.h"

namespace EclipseCompiler
{
	enum class GeometryIndex : int
	{
		GI_NONE = 0,
		GI_MESHNAME = 1,
		GI_NOTEX = 2,
		GI_DIFFUSE = 3,
		GI_SPECULAR = 4,
		GI_AMBIENT = 5,

		GI_VERTICES_POSITION = 6,

		GI_MAXCOUNT
	};

	class MeshA
	{
	public:
		glm::vec4               Diffuse{ 30,40,50,60 };
		glm::vec4               Specular{ 70,80,90,70 };
		glm::vec4               Ambient{ 1,2,3,4 };
		std::array<char, 128>   MeshName;
		bool                    noTex{ true };
		std::vector<Vertex>     Vertices;
		std::vector<unsigned int> Indices;

		MeshA()
		{
			strcpy_s(MeshName.data(), MeshName.size(), "poop");
			MeshName[5] = '\0';
		};

		~MeshA() { };
	};

	struct Mesh
	{
	public:
		char* MeshName;
		bool NoTex{ false };
		glm::vec4 Diffuse{ 0,0,0,0 };
		glm::vec4 Specular{ 0,0,0,0 };
		glm::vec4 Ambient;
		std::vector<Vertex> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<Texture> Textures;

		Mesh()
		{
			MeshName = new char[20];
			strcpy_s(MeshName, 5, "poop");
			MeshName[5] = '\0';

			offSetsforObject = {
	0,
	20,
	sizeof(NoTex),
	sizeof(Diffuse),
	sizeof(Specular),
	sizeof(Ambient),
	Vertices.size() * sizeof(Vertex),
	Indices.size() * sizeof(unsigned int)
			};

			//strcpy_s(MeshName, 20, "");
			//MeshName[20] = '\0';
		};

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string namein, std::vector<Texture> textures)
			:
			Vertices(vertices),
			Indices(indices),
			Textures(textures),
			NoTex(false)
		{
			//auto l = namein.length() + 1;
			//MeshName = new char[namein.length()+1];
			//strcpy_s(MeshName, namein.length() + 1, namein.c_str());
			////strcpy(MeshName, namein.c_str());
			//MeshName[namein.length()] = '\0';

			MeshName = new char[20];
			strcpy_s(MeshName, 5, "poop");
			MeshName[5] = '\0';

			offSetsforObject = {
				0,
				20,
				sizeof(NoTex),
				sizeof(Diffuse),
				sizeof(Specular),
				sizeof(Ambient),
				Vertices.size() * sizeof(Vertex),
				Indices.size() * sizeof(unsigned int)
			};
		}

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::vec4  diffuse, glm::vec4  specular, glm::vec4  ambient, bool in, std::string namein)
			:
			Vertices(vertices),
			Indices(indices),
			Diffuse(diffuse),
			Specular(specular),
			Ambient(ambient),
			NoTex(in)
		{
			//auto l = namein.length() + 1;
			//MeshName = new char[namein.length() + 1];
			//strcpy_s(MeshName, namein.length() + 1, namein.c_str());
			////strcpy(MeshName, namein.c_str());
			//MeshName[namein.length()] = '/0';

			MeshName = new char[20];
			strcpy_s(MeshName, 5, "poop");
			MeshName[5] = '\0';

			offSetsforObject = {
				0,
				20,
				sizeof(NoTex),
				sizeof(Diffuse),
				sizeof(Specular),
				sizeof(Ambient),
				Vertices.size() * sizeof(Vertex),
				Indices.size() * sizeof(unsigned int)
			};
		}

		~Mesh() {};

		std::vector<size_t> offSetsforObject;
	};

	struct MeshData
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		glm::vec4 Diffuse;
		glm::vec4 Specular;
		glm::vec4 Ambient;
		bool NoTextures = false;
		const char* MeshName;
		std::vector<Texture> textures;
	};

	class AssimpLoader
	{
	public:
		unsigned int MeshIndex = 0;
		std::string NameOfModel;
		std::string Directory;
		std::vector<Mesh> Meshes;
		std::vector<glm::vec3> AllVertices;
		std::vector<MeshData> meshData;
		std::vector<Texture> Textures_loaded;

	public:

		void LoadAssimpModel(std::string path, std::unordered_map<std::string, Mesh>& GeometryContainer);
		void ProcessGeometry(aiNode* node, const aiScene* scene, bool isGeometryCompiler = true);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene, const char* MeshName);
		float GetLargestAxisValue(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
		void ComputeAxisMinMax(std::vector<glm::vec3>& vertices, std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
		glm::vec3 ComputeCentroid(std::pair<float, float>& _minmaxX, std::pair<float, float>& _minmaxY, std::pair<float, float>& _minmaxZ);
		void LoadNewModel(std::unordered_map<std::string, Mesh>& GeometryContainer);

	public:
		std::vector<Texture> ExtractTextures(aiMesh* mesh, const aiScene* scene, std::string& MeshName, std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>&);
		void LoadAssimpModelForTextures(std::string path, std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>&);
		void ProcessTextures(aiNode* node, const aiScene* scene, std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>& TextureContainer);
		std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type, const char* MeshName);
		std::vector<Texture> LoadTexturesForCompiler(aiMaterial* mat, aiTextureType type, std::string& MeshName, std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<std::unique_ptr<Texture>>>>&);
	};
}