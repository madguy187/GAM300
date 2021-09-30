#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>

namespace Eclipse
{
	// Define available file changes
	enum class FileStatus
	{
		FS_NONE = 0,
		FS_CREATED = 1,
		FS_MODIFIED = 2,
		FS_ERASED = 3,
		MAXCOUNT
	};

	class EclipseFileWatcher
	{
	public:
		std::string PathToWatch;
		std::chrono::duration<int, std::milli> Delays;
		std::unordered_map<std::string, std::filesystem::file_time_type> paths_;

		EclipseFileWatcher();
		EclipseFileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay);
		void Start(const std::function<void(std::string, FileStatus)>& action);
		bool Contains(const std::string& key);

	};
}