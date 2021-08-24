#pragma once

namespace Eclipse
{
	class DragAndDrop
	{
	public:
		void GenericPayloadSource(const char* id, const std::string& source, 
			PayloadSourceType type = PayloadSourceType::PST_TEXT);

		void GenericPayloadTarget(const char* id, std::string& destination, 
			const char* cMsg, PayloadTargetType type = PayloadTargetType::PTT_WIDGET);
		void AssetBrowerFilesAndFoldersTarget(const char* type, const char* paths, std::string AssetPath, 
			std::filesystem::directory_entry dirEntry, bool refreshBrowser);
	};
}
