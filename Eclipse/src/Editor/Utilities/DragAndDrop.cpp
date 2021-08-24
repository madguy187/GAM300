#include "pch.h"
#include "DragAndDrop.h"
namespace Eclipse
{
	void DragAndDrop::AssetBrowerFilesAndFoldersSource(const char* type, std::string relativePath)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(type, relativePath.c_str(), ((relativePath.size()) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}
	}
	
	void DragAndDrop::AssetBrowerFilesAndFoldersTarget(const char* type, const char* paths, std::string AssetPath,std::filesystem::directory_entry dirEntry,bool refreshBrowser)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type))
			{
				try
				{
					paths = (const char*)payload->Data;
					std::filesystem::path itemPaths = AssetPath.c_str();
					std::filesystem::copy(std::filesystem::path(itemPaths / paths), dirEntry.path());
					std::filesystem::remove(std::filesystem::path(itemPaths / paths));
					refreshBrowser = true;
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