#pragma once
#include <filesystem>
#include "../Interface/ECGuiWindow.h"
namespace Eclipse
{
	class AssetBrowserWindow final : public ECGuiWindow
	{
		const std::filesystem::path AssetPath = "src//Assets";
		std::filesystem::path AllDir;
		std::filesystem::path CurrentDir;
		std::filesystem::path NextDir;
		RenderComponent sprite;
		RenderComponent FolderIcon;
		char searchBuffer[128];
		std::vector<std::filesystem::path> SearchPaths;

	public:
		void Update() override;
		AssetBrowserWindow();
		void DrawImpl();
		void LeftFolderHierarchy();
		void RightFoldersAndItems();
		void FoldersAndItems();
		void PathTracker();
		void BackToParentPath(std::filesystem::path &CurrentPath);
		void NextPath(std::filesystem::path& CurrentPath,std::filesystem::path paths);
	};
}