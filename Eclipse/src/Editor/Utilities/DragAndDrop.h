#pragma once

namespace Eclipse
{
	class DragAndDrop
	{
		std::map< std::string, std::string> files;
		std::map< std::string, std::string> deletefiles;
	public:
		void StringPayloadSource(const char* id, const std::string& source,
			PayloadSourceType type = PayloadSourceType::PST_TEXT);
		void IndexPayloadSource(const char* id, const int& source,
			PayloadSourceType type = PayloadSourceType::PST_UNASSIGNED);

		void StringPayloadTarget(const char* id, std::string& destination,
			const char* cMsg, PayloadTargetType type = PayloadTargetType::PTT_WIDGET);
		void IndexPayloadTarget(const char* id, const int& destination,
			const char* cMsg, PayloadTargetType type = PayloadTargetType::PTT_INDEXSWAPPING);
		void AssetBrowerFilesAndFoldersTarget(const char* type, const char* paths, std::string AssetPath, 
			std::filesystem::directory_entry dirEntry, bool& refreshBrowser, std::map<std::filesystem::path, 
			std::vector<std::filesystem::path>> pathMap);
	};
}
