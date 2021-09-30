#include "pch.h"
#include "../FileWatchSystem/FileWatchSystem.h"
#include "Graphics/FileWatch/FileWatch.h"

namespace Eclipse
{
	void FileWatchSystem::Init()
	{
		EclipseFileWatcher watch{ "src/Assets/Compilers", engine->gFileWatchManager->InputTime(5) };
		engine->gFileWatchManager = std::make_unique<EclipseFileWatcher>(watch);
	}

	void FileWatchSystem::Update()
	{
		// I only run in editor state
		if (engine->GetEditorState())
		{
			if (engine->gFileWatchManager->UpdateTimer())
			{
				std::cout << "Checking every 5 seconds" << std::endl;

				engine->gFileWatchManager->Start([](std::string PATH_TO_WATCH, FileStatus status) -> void
					{
						std::cout << "Checking " << std::endl;

						if (!std::filesystem::is_regular_file(std::filesystem::path(PATH_TO_WATCH)) && status != FileStatus::FS_ERASED)
						{
							return;
						}

						switch (status)
						{
						case FileStatus::FS_CREATED:
							std::cout << "File created: " << PATH_TO_WATCH << '\n';
							//engine->AssimpManager.HotReload();
							break;
						case FileStatus::FS_MODIFIED:
							std::cout << "File modified: " << PATH_TO_WATCH << '\n';
							//engine->AssimpManager.HotReload();
							break;
						case FileStatus::FS_ERASED:
							std::cout << "File erased: " << PATH_TO_WATCH << '\n';
							//engine->AssimpManager.HotReload();
							break;
						default:
							std::cout << "Error! Unknown file status.\n";

						}
					}
				);
			}
		}
		//engine->AssimpManager.ResetHotReloadFlag();
	}
}