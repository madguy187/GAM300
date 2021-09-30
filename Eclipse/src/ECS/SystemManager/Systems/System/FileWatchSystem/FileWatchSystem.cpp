#include "pch.h"
#include "../FileWatchSystem/FileWatchSystem.h"
#include "Graphics/FileWatch/FileWatch.h"

namespace Eclipse
{
	void FileWatchSystem::Init()
	{
		EclipseFileWatcher fw{ "src/Assets/Compilers", std::chrono::milliseconds(5000) };
		engine->gFileWatchManager = std::make_unique<EclipseFileWatcher>(fw);
	}

	void FileWatchSystem::Update()
	{
		engine->gFileWatchManager->Start([](std::string PATH_TO_WATCH, FileStatus status) -> void
			{
				if (!std::filesystem::is_regular_file(std::filesystem::path(PATH_TO_WATCH)) && status != FileStatus::FS_ERASED)
				{
					return;
				}

				switch (status) 
				{
				case FileStatus::FS_CREATED:
					std::cout << "File created: " << PATH_TO_WATCH << '\n';
					break;
				case FileStatus::FS_MODIFIED:
					std::cout << "File modified: " << PATH_TO_WATCH << '\n';
					break;
				case FileStatus::FS_ERASED:
					std::cout << "File erased: " << PATH_TO_WATCH << '\n';
					break;
				default:
					std::cout << "Error! Unknown file status.\n";

				}
			});
	}
}