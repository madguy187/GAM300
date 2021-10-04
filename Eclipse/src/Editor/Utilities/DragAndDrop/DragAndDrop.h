#pragma once

namespace Eclipse
{
	static const char* IndexJobNames[] = { "Move", "Swap", "Parent Child", "Cancel"};

	class DragAndDrop
	{
	public:
		void StringPayloadSource(const char* id, const std::string& source,
			PayloadSourceType type = PayloadSourceType::PST_TEXT);
		void IndexPayloadSource(const char* id, const int& source,
			PayloadSourceType type = PayloadSourceType::PST_UNASSIGNED);

		void StringPayloadTarget(const char* id, std::string& destination,
			const char* cMsg, PayloadTargetType type = PayloadTargetType::PTT_WIDGET, Entity ID = MAX_ENTITY + 1, size_t arrayIndex = -1);
		void IndexPayloadTarget(const char* id, const int& destination, bool IsSelected,
			PayloadTargetType type = PayloadTargetType::PTT_INDEXEDIT);
		void AssetBrowerFilesAndFoldersTarget(const char* type, const char* paths, std::string AssetPath, 
			std::filesystem::directory_entry dirEntry, bool& refreshBrowser, std::map<std::filesystem::path, 
			std::vector<std::filesystem::path>> pathMap,bool& CopyMode);

		void ClearAllFiles(std::string& folderName, std::string& parentPath, bool& refreshBrowser);
		void CreateEmptyFolder(std::string folderName ,std::string folderPath = "src//Assets\\");
	private:
		std::map< std::string, std::string> files;
		std::map< std::string, std::string> deletefiles;
		bool IsIndexJobSelected{ false };
		int SourceIndex_{ 0 };
		int DestinationIndex_{ 0 };
	};
}
