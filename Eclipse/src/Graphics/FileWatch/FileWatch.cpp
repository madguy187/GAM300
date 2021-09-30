#include "pch.h"
#include "Graphics/FileWatch/FileWatch.h"

namespace Eclipse
{
	// Default Constructor for me to init
	EclipseFileWatcher::EclipseFileWatcher()
	{

	}

	EclipseFileWatcher::EclipseFileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay) :
		PathToWatch{ path_to_watch },
		Delays{ delay }
	{
		for (auto& file : std::filesystem::recursive_directory_iterator(path_to_watch))
		{
			paths_[file.path().string()] = std::filesystem::last_write_time(file);

		}
	}

	void EclipseFileWatcher::Start(const std::function<void(std::string, FileStatus)>& action)
	{
		// Wait for "delay" milliseconds
		//std::this_thread::sleep_for(delay);

		auto it = paths_.begin();

		while (it != paths_.end())
		{
			if (!std::filesystem::exists(it->first))
			{
				action(it->first, FileStatus::FS_ERASED);
				it = paths_.erase(it);

			}
			else {
				it++;

			}

		}

		// Check if a file was created or modified
		for (auto& file : std::filesystem::recursive_directory_iterator(PathToWatch))
		{
			auto current_file_last_write_time = std::filesystem::last_write_time(file);

			// File creation
			if (Contains(file.path().string()) == false)
			{
				paths_[file.path().string()] = current_file_last_write_time;
				action(file.path().string(), FileStatus::FS_CREATED);
				// File modification
			}
			else
			{
				if (paths_[file.path().string()] != current_file_last_write_time)
				{
					paths_[file.path().string()] = current_file_last_write_time;
					action(file.path().string(), FileStatus::FS_MODIFIED);
				}

			}

		}
	}

	bool EclipseFileWatcher::Contains(const std::string& key)
	{
		auto el = paths_.find(key);
		return el != paths_.end();

	}
}