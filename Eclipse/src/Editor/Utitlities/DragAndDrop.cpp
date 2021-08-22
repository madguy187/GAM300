#include "pch.h"
#include "DragAndDrop.h"
namespace Eclipse
{
	void DragAndDrop::AssetBrowerFilesAndFoldersSource(const char* type, std::string relativePath)
	{
		if (ImGui::BeginDragDropSource())
		{
			wchar_t itemPath[100];
			mbstowcs(itemPath, relativePath.c_str(), 99);
			ImGui::SetDragDropPayload(type, itemPath, ((relativePath.size()) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}
	}
	void DragAndDrop::AssetBrowerFilesAndFoldersTarget(const char* type, const wchar_t* paths, std::string AssetPath,std::filesystem::directory_entry dirEntry,bool refreshBrowerser)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type))
			{
				try
				{
					paths = (const wchar_t*)payload->Data;
					wchar_t itemPath[100];
					mbstowcs(itemPath, AssetPath.c_str(), 99);
					std::filesystem::path itemPaths = itemPath;
					std::filesystem::copy(std::filesystem::path(itemPaths / paths), dirEntry.path());
					std::filesystem::remove(std::filesystem::path(itemPaths / paths));
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