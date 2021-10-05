#pragma once

namespace Eclipse
{
	class EngineCompiler
	{
	private:
		std::vector<std::string> AllNames = { "GEOMETRY" , "PREFABS" , "BASICTEXTURES" , "TEXTURES" };
		// Touch Only When Needed
		std::string GeometryPath = "../Compiler/CompilerKeyFiles/GeometryFile/Geometry.eclipse";
		std::string PrefabsPath = "../Compiler/CompilerKeyFiles/PrefabsFile/Prefabs.eclipse";
		std::string ModelTexturePath = "../Compiler/CompilerKeyFiles/TextureFile/Texture.eclipse";
		std::string TexturePath = "../Compiler/CompilerKeyFiles/BasicTextureFile/Texture.eclipse";
		// Loading Geometry.eclipse File
		std::fstream GeometryFileRead;
		// Loading Prefabs.eclipse File
		std::fstream PrefabsFileRead;
		// Loading Textures.eclipse File
		std::fstream TextureFileRead;
		// Loading Textures.eclipse File
		std::fstream ModelTextureFileRead;
		std::bitset<4> CompilerFlags;

		// Load Geometries
		void LoadGeometry();
		// Load Prefabs - This will Store all the Prefabs Name
		void LoadPrefabs();
		// Load Basic Textures
		void LoadBasicTextures();
		// Load Model Textures 
		void LoadModelTextures();
		// Execute Compiler
		static void RunCompiler();
		void LoadCompilers();
		bool OpenFile(std::fstream& FileIn, std::string& Path);
		void CloseFile(std::fstream& FileIn, std::string& FileName);
	public:
		EngineCompiler();
		bool IsGeometryCompiled();
		bool IsPrefabsCompiled();
		bool IsModelTexturesCompiled();
		bool IsBasicTexturesCompiled();
		bool AreAllCompiled();
		void HotReload();
		void HotReloadTetxures();
		void Init();
	};
}