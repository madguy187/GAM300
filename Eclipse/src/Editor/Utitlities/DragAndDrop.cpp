#include "pch.h"
#include "DragAndDrop.h"
namespace Eclipse
{
	void DragAndDrop::AssetBrowerFilesAndFoldersSource(const char* type, std::filesystem::path relativePath)
	{
		if (ImGui::BeginDragDropSource())
		{
			const wchar_t* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload(type, itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}
	}
	void DragAndDrop::AssetBrowerFilesAndFoldersTarget(const char* type, const wchar_t* paths, std::filesystem::path AssetPath,std::filesystem::directory_entry dirEntry,bool refreshBrowerser)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type))
			{
				try
				{
					paths = (const wchar_t*)payload->Data;
					std::filesystem::copy(std::filesystem::path(AssetPath / paths), dirEntry.path());
					std::filesystem::remove(std::filesystem::path(AssetPath / paths));
					refreshBrowerser = true;
				}
				catch (std::filesystem::filesystem_error& e)
				{
					std::cout << e.what() << '\n';
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
}