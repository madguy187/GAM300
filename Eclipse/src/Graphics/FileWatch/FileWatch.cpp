#include "pch.h"
#include "Graphics/FileWatch/FileWatch.h"

namespace Eclipse
{
	bool FileWatcher::contains(const std::string& key)
	{
		auto el = paths_.find(key);
		return el != paths_.end();
	}

	FileWatcher::FileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay) :
		path_to_watch(path_to_watch),
		delay(delay)
	{
		for (auto& file : std::filesystem::recursive_directory_iterator(path_to_watch))
		{
			paths_[file.path().string()] = std::filesystem::last_write_time(file);
		}
	}

}