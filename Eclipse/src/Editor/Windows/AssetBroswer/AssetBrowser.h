#pragma once
#include <filesystem>
#include "../Interface/ECGuiWindow.h"
namespace Eclipse
{
	class AssetBrowser final : public ECGuiWindow
	{
		const std::filesystem::path AssetPath = "Assets";
		std::filesystem::path CurrentDir;
		std::filesystem::path NextDir;

	public:
		void Update() override;
		AssetBrowser();
		void DrawImpl();
		void PathTracker(std::filesystem::path& CurrentPath);
		void BackToParentPath(std::filesystem::path &CurrentPath);
		void NextPath(std::filesystem::path& CurrentPath,std::filesystem::path paths);






	};
}