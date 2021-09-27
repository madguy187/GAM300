#include "pch.h"
#include "GeometryCompiler.h"
#include <string> 


//struct Class1
//{
//    int x;
//    float y;
//    float z;
//    std::string a = "11111111111111111111";
//
//    std::vector<size_t> offSetsforObject;
//
//    Class1()
//    {
//        offSetsforObject = { 4, sizeof(y), 4, sizeof(a) };
//    }
//};
//
//struct FileManager
//{
//    Class1* obj = new Class1;
//    std::ofstream GeometryFile;
//
//    for (int i = 0; i < 3; ++i)
//    {
//        char* address = reinterpret_cast<char*>(obj) + obj->offSetsforObject[i];
//        GeometryFile.write(reinterpret_cast<const char*>(address), obj->offSetsforObject[i]);
//    }
//};

//    void WriteToBinary(const char* path)
//    {
//
//       f.open("../Eclipse/src/Assets/Compilers/GeometryFile/Geometry.bin",
//                std::fstream::out |
//                std::fstream::binary |
//                std::ios::trunc |
//                std::ios::binary);
//
//        GeometryFile.write(reinterpret_cast<const char*>(&x), sizeof(int));
//        GeometryFile.write(reinterpret_cast<const char*>(&y), sizeof(float));
//        GeometryFile.write(reinterpret_cast<const char*>(&z), sizeof(float));
//        GeometryFile.close();
//    }
//    void ReadFile()
//    {
//        GeometryFile.write(reinterpret_cast<const char*>(&x), sizeof(int));
//        GeometryFile.write(reinterpret_cast<const char*>(&y), sizeof(float));
//        GeometryFile.write(reinterpret_cast<const char*>(&z), sizeof(float));
//    }

namespace EclipseCompiler
{
	void GeometryCompiler::Init()
	{
		LoadFile("..//Eclipse//src//Assets//ASSModels");
	}

	void GeometryCompiler::LoadFile(const std::string& modelFile)
	{
		for (auto& dirEntry : std::filesystem::directory_iterator(modelFile))
		{
			const auto& path = dirEntry.path();
			auto relativePath = relative(path, "..//Eclipse//src//");
			std::string FolderName = relativePath.filename().string();

			std::string GoIntoModelFolder = ("..//Eclipse//src/Assets/ASSModels/" + FolderName);

			for (auto& dirEntry : std::filesystem::directory_iterator(GoIntoModelFolder))
			{
				const auto& FbxOrGltf = dirEntry.path();
				auto relativePath = relative(FbxOrGltf, "..//Eclipse//src//");
				std::string FbxOrGltfName = relativePath.filename().string();

				if (FbxOrGltfName.find("gltf") != std::string::npos || FbxOrGltfName.find("fbx") != std::string::npos)
				{
					std::string PathName = ("..//Eclipse//src/Assets/ASSModels/" + FolderName + "/" + FbxOrGltfName).c_str();
					std::unique_ptr<AssimpLoader> ptr = std::make_unique< AssimpLoader>();
					ptr->LoadAssimpModel(PathName, Geometry);
				}
			}
		}
	}

	void GeometryCompiler::WriteToFile(std::unordered_map<std::string, std::unique_ptr<Mesh>>& In)
	{
		GeometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			GeometryFile.open("../Eclipse/src/Assets/Compilers/GeometryFile/Geometry.bin",
				std::fstream::out |
				std::ios::trunc |
				std::ios::binary);
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "Exception opening/reading file" << e.what();
		}

		#ifndef NEW_IMPLEMENTATION
		//int LoopCounter = 0;
		//std::unordered_map<std::string, std::unique_ptr<Mesh>>::iterator hi;

		for (auto i = Geometry.begin() ; i != Geometry.end() ; i++ )
		{
			std::cout <<  std::addressof(i->second.get()->MeshName) << std::endl;
			std::cout << ((unsigned char*)i->second.get()) << std::endl;

			//// debug this
			//for (size_t Internal = 0; Internal < 1 ; ++Internal)
			//{
			//	//auto mesh = reinterpret_cast<void*>(i->second.get());
			//	//char* Start = reinterpret_cast<char*>(mesh) + i->second->offSetsforObject[Internal];
			//	GeometryFile.write(reinterpret_cast<const char*>(&i->second->offSetsforObject[Internal + 1]), sizeof(int));
			//	GeometryFile.write(reinterpret_cast<const char*>(i->second.get()), i->second->offSetsforObject[Internal + 1]);
			//}

			//// Mesh Name
			//int SizeOfMeshName = std::strlen(i.second->MeshName)+1;
			//GeometryFile.write(reinterpret_cast<const char*>(&SizeOfMeshName), sizeof(int));
			//GeometryFile.write(reinterpret_cast<const char*>(i.second->MeshName), SizeOfMeshName);

			//// HasTexture
			//int SizeOfBoolean = sizeof(NoTextures);
			//GeometryFile.write(reinterpret_cast<const char*>(&SizeOfBoolean), sizeof(bool));
			//GeometryFile.write(reinterpret_cast<const char*>(&i.second->NoTex), SizeOfBoolean);

		}
		GeometryFile.close();

		ReadFile();
#else
		for (auto& i : In)
		{
			// Name , NoTextures , Diffuse , Specular , Ambient
			GeometryFile << i.first << std::endl;
			GeometryFile << (i.second->NoTex ? 1 : 0) << std::endl;
			GeometryFile << i.second->Diffuse.x << " " << i.second->Diffuse.y << " " << i.second->Diffuse.z << " " << i.second->Diffuse.w << std::endl;
			GeometryFile << i.second->Specular.x << " " << i.second->Specular.y << " " << i.second->Specular.z << " " << i.second->Specular.w << std::endl;
			GeometryFile << i.second->Ambient.x << " " << i.second->Ambient.y << " " << i.second->Ambient.z << " " << i.second->Ambient.z << std::endl;
			GeometryFile << "-" << std::endl;

			// Vertices
			// Position , Normal , TextureCoordinates
			for (auto& Vertices : i.second->Vertices)
			{
				GeometryFile << Vertices.Position.x << " " << Vertices.Position.y << " " << Vertices.Position.z << std::endl;
				GeometryFile << Vertices.Normal.x << " " << Vertices.Normal.y << " " << Vertices.Normal.z << std::endl;
				GeometryFile << Vertices.TextureCoodinates.x << " " << Vertices.TextureCoodinates.y << std::endl;
			}
			GeometryFile << "--" << std::endl;

			for (auto& Indices : i.second->Indices)
			{
				GeometryFile << Indices << std::endl;
			}
			GeometryFile << "---" << std::endl;

		}
		GeometryFile.close();
#endif
	}

	void GeometryCompiler::ReleaseFile(std::string& in)
	{
		if (in == "1")
		{
			if (Geometry.empty())
			{
				std::cout << "No Geometry Loaded" << std::endl << std::endl;
				return;
			}

			WriteToFile(Geometry);
			std::cout << "Geometry File Produced" << std::endl << std::endl;
		}
	}

	void GeometryCompiler::Write()
	{

	}

	void GeometryCompiler::CalculateOffsets()
	{
		for (auto& Model : Geometry)
		{
			OffSetsForObject.push_back(sizeof(Model.first));
			OffSetsForObject.push_back(sizeof(Model.second->NoTex));
			//OffSetsForObject.push_back(sizeof(Model.second->Diffuse));
			//OffSetsForObject.push_back(sizeof(Model.second->Specular));
			//OffSetsForObject.push_back(sizeof(Model.second->Ambient));
			//OffSetsForObject.push_back(Model.second->Vertices.size() * sizeof(Vertex));
			//OffSetsForObject.push_back(Model.second->Indices.size() * sizeof(unsigned int));
			//OffsetContainer.push_back(OffSetsForObject);
		}
	}

	void GeometryCompiler::ReadFile()
	{
		GeometryFileWrite.open("../Eclipse/src/Assets/Compilers/GeometryFile/Geometry.bin",
			std::ios::in |
			std::ios::binary);

		std::string eachline;

#ifndef NEW_IMPLEMENTATION

		Mesh TestCreateModelFromCompiler;

		int MeshNameSize = 0;
		TestCreateModelFromCompiler.MeshName = new char[MeshNameSize];
		GeometryFileWrite.read(reinterpret_cast<char*>(&MeshNameSize), sizeof(int));
		GeometryFileWrite.read(reinterpret_cast<char*>(TestCreateModelFromCompiler.MeshName), MeshNameSize);

		//int NoTextureSize = 0;
		//GeometryFileWrite.read(reinterpret_cast<char*>(&NoTextureSize), sizeof(bool));
		//GeometryFileWrite.read(reinterpret_cast<char*>(&TestCreateModelFromCompiler.NoTex), NoTextureSize);

		//int TestDiffuseSize = 0;
		//glm::vec3 TestDiffuse;
		//GeometryFileWrite.read(reinterpret_cast<char*>(&TestDiffuseSize), sizeof(glm::vec3));
		//GeometryFileWrite.read(reinterpret_cast<char*>(&TestDiffuse), TestDiffuseSize);

		int i = 0;

#else   
		// This getline is to getName
		while (!GeometryFile.eof())
		{
			std::getline(GeometryFileWrite, eachline);

			if (eachline == "")
			{
				break;
			}

			Mesh Model;

			// Name
			Model.MeshName = eachline;

			// NoTextures
			std::getline(GeometryFileWrite, eachline);
			Model.NoTex = std::atoi(eachline.c_str());

			// Diffuse
			std::getline(GeometryFileWrite, eachline);
			ProcessVec4(eachline, Model.Diffuse);

			// Specular
			std::getline(GeometryFileWrite, eachline);
			ProcessVec4(eachline, Model.Specular);

			// Ambient
			std::getline(GeometryFileWrite, eachline);
			ProcessVec4(eachline, Model.Ambient);

			std::getline(GeometryFileWrite, eachline);
			//if (eachline == "-")
				//std::cout << "Vertices Next" << std::endl;

			while (std::getline(GeometryFileWrite, eachline))
			{
				if (eachline == "--")
				{
					//std::cout << "Indices Next" << std::endl;
					break;
				}

				Vertex A;
				A.Position = ReturnVec3(eachline);

				std::getline(GeometryFileWrite, eachline);
				A.Normal = ReturnVec3(eachline);

				std::getline(GeometryFileWrite, eachline);
				A.TextureCoodinates = ReturnVec2(eachline);

				Model.Vertices.push_back(A);
			}

			while (std::getline(GeometryFileWrite, eachline))
			{
				if (eachline == "---")
				{
					//std::cout << "Next Model" << std::endl;
					break;
				}

				Model.Indices.push_back(std::atoi(eachline.c_str()));
			}

			Geometry.emplace(Model.MeshName, std::make_unique<Mesh>(Model));
		}
#endif
	}
}