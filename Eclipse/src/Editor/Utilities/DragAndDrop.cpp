#include "pch.h"
#include "DragAndDrop.h"

namespace Eclipse
{
	void DragAndDrop::GenericPayloadSource(const char* id, const std::string& source,
		PayloadSourceType type)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(id, source.c_str(), source.size() + 1);

			switch (type)
			{
				case PayloadSourceType::PST_TEXT:
					ImGui::TextUnformatted(source.c_str());
					break;
				case PayloadSourceType::PST_IMAGE:
					// For rendering 2D Image -> Need ask Graphics side
					break;
				default:
					break;
			}

			ImGui::EndDragDropSource();
		}
	}

	void DragAndDrop::GenericPayloadTarget(const char* id, std::string& destination,
		const char* cMsg, PayloadTargetType type)
	{
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(id);

			if (payload)
			{
				switch (type)
				{
				case PayloadTargetType::PTT_WIDGET:
					destination = (const char*)payload->Data;
					break;
				// Create Meshes and instances
				case PayloadTargetType::PTT_WINDOW:
					// Load Stuff here in the future
					break;
				default:
					break;
				}

				EDITOR_LOG_INFO(cMsg);
			}

			ImGui::EndDragDropTarget();
		}
	}
	
	void DragAndDrop::AssetBrowerFilesAndFoldersTarget(const char* type, const char* paths, 
		std::string AssetPath, std::filesystem::directory_entry dirEntry, bool refreshBrowser)
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
