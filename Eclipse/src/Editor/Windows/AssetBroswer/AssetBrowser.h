#pragma once
#include <filesystem>
#include "../Interface/ECGuiWindow.h"
namespace Eclipse
{
	class AssetBrowserWindow final : public ECGuiWindow
	{
		const std::filesystem::path AssetPath = "src//Assets";
		std::filesystem::path CurrentDir;
		std::filesystem::path NextDir;

	public:
		void Update() override;
		AssetBrowserWindow();
		void DrawImpl();
		void PathTracker(std::filesystem::path& CurrentPath);
		void BackToParentPath(std::filesystem::path &CurrentPath);
		void NextPath(std::filesystem::path& CurrentPath,std::filesystem::path paths);






	};
}