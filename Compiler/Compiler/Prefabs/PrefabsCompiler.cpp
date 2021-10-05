#include "pch.h"
#include "PrefabsCompiler.h"

namespace EclipseCompiler
{
	void PrefabsCompiler::WriteToFile()
	{
		PrefabsFileWrite.open(Path + "PrefabsFile/Prefabs" + FileName,
			std::ios_base::out |
			std::ios_base::trunc |
			std::ios_base::binary);

		if (PrefabsFileWrite.fail())
		{
			std::cout << "Fail To Open Prefabs File" << std::endl << std::endl;
			return;
		}

		std::cout << "Writing to Prefabs File " << std::endl;
		int NumberOfPrefabs = Prefabs.size();
		PrefabsFileWrite.write(reinterpret_cast<const char*>(&NumberOfPrefabs), sizeof(NumberOfPrefabs));
		std::cout << "Detected Prefabs Size " << NumberOfPrefabs << std::endl;
		for (auto i : Prefabs)
		{
			std::array<char, 128> ParentName;
			strcpy_s(ParentName.data(), ParentName.size(), i.first.data());
			ParentName[ParentName.size() - 1] = '\0';
			PrefabsFileWrite.write(reinterpret_cast<const char*>(&ParentName), sizeof(ParentName));

			int NumberOfSubMeshes = i.second.size();
			PrefabsFileWrite.write(reinterpret_cast<const char*>(&NumberOfSubMeshes), sizeof(NumberOfSubMeshes));

			for (auto EachMeshName : i.second)
			{
				std::array<char, 128> MeshName;
				strcpy_s(MeshName.data(), MeshName.size(), EachMeshName.data());
				MeshName[MeshName.size() - 1] = '\0';
				PrefabsFileWrite.write(reinterpret_cast<const char*>(&MeshName), sizeof(MeshName));
			}
		}
		PrefabsFileWrite.close();
		std::cout << "Done Writing to Prefabs File " << std::endl;
	}

	void PrefabsCompiler::ReadFile()
	{
		PrefabsFileRead.open(Path + "PrefabsFile/Prefabs" + FileName,
			std::ios::in |
			std::ios::binary);

		if (PrefabsFileRead.fail())
		{
			std::cout << "Fail To Open Prefabs File" << std::endl << std::endl;
			return;
		}

		std::unordered_map<std::string, std::vector<std::string>> Test;

		int TotalNumberOfPrefabs = 0;

		// See how many Prefabs
		// Fikrul was here
		PrefabsFileRead.read(reinterpret_cast<char*>(&TotalNumberOfPrefabs), sizeof(int));

		for (int i = 0; i < TotalNumberOfPrefabs; i++)
		{
			std::array<char, 128> ParentName;
			PrefabsFileRead.read(reinterpret_cast<char*>(&ParentName), sizeof(ParentName));
			ParentName[ParentName.size() - 1] = '\0';

			int NumberOfSubMeshes = 0;
			PrefabsFileRead.read(reinterpret_cast<char*>(&NumberOfSubMeshes), sizeof(NumberOfSubMeshes));

			for (int i = 0; i < NumberOfSubMeshes; i++)
			{
				std::array<char, 128> MeshName;
				PrefabsFileRead.read(reinterpret_cast<char*>(&MeshName), sizeof(MeshName));
				MeshName[MeshName.size() - 1] = '\0';

				Test[ParentName.data()].push_back(MeshName.data());
			}
		}
		PrefabsFileRead.close();
	}

	void PrefabsCompiler::LoadFile(const std::string& modelFile)
	{

	}

	void PrefabsCompiler::Init()
	{

	}

	void PrefabsCompiler::ReleaseFile()
	{
		if (Prefabs.empty())
		{
			std::cout << "No Models Loaded" << std::endl << std::endl;
			return;
		}

		WriteToFile();
		std::cout << "Prefabs File Compiled" << std::endl << std::endl;
	}

	void PrefabsCompiler::ReadFile(std::string& in)
	{
		if (in == "Pre")
		{
			if (Prefabs.empty())
			{
				std::cout << "No Models Loaded" << std::endl << std::endl;
				return;
			}

			ReadFile();
		}
	}

	void PrefabsCompiler::GetPrefabs(std::unordered_map<std::string, std::vector<std::string>>& in)
	{
		Prefabs = in;
	}
}
