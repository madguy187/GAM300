#pragma once
#include <filesystem>
#include "../Interface/ECGuiWindow.h"
namespace Eclipse
{
	class AssetBrowser final : public ECGuiWindow
	{
		std::filesystem::path m_CurrentDir;
		std::filesystem::path m_SecondDir;

	public:
		void Update() override;
		AssetBrowser();
		void DrawImpl();
		void BackToParentPath(std::filesystem::path &CurrentPath);
		std::filesystem::path NextPath(std::filesystem::path CurrentPath,std::filesystem::path paths);






	};
}