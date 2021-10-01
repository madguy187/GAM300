#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>

namespace Eclipse
{
	// I can only check these statuses
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
		bool Modified = false;
		float Timer = 0.0f;
		float HotReloadCooldown = 0.0f;
		float HardResetTime = 5.0f;
		unsigned int AssetCounter = 0;
		unsigned int BasicTextureCounter = 0;
		std::string PathToWatch;
		std::chrono::duration<int, std::milli> Delays;
		std::unordered_map<std::string, std::filesystem::file_time_type> paths_;

		EclipseFileWatcher();
		EclipseFileWatcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay);
		void Start(const std::function<void(std::string, FileStatus)>& action);
		bool Contains(const std::string& key);
		std::chrono::duration<int, std::milli> InputTime(unsigned int in);
		bool UpdateTimer();
		bool CheckBasicTexture(std::string& in);
		void Resolutions(FileStatus status, std::string& PATH_TO_WATCH);
		void HardReset(float in);
	};
}