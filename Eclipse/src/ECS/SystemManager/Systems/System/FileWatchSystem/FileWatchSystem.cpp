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
                engine->gFileWatchManager->Start([](std::string PATH_TO_WATCH, FileStatus status) -> void
                {
                    if (!std::filesystem::is_regular_file(std::filesystem::path(PATH_TO_WATCH)) && status != FileStatus::FS_ERASED)
                    {
                        return;
                    }

                    // Hot Reloading
                    if (engine->AssimpManager.GetHotReloadFlag())
                    {
                        return;
                    }

                    switch (status)
                    {
                    case FileStatus::FS_CREATED:
                        std::cout << "File created: " << PATH_TO_WATCH << '\n';
                        break;
                    case FileStatus::FS_MODIFIED:
                    {
                        std::cout << "File modified: " << PATH_TO_WATCH << '\n';
                        engine->AssimpManager.HotReload();
                        engine->gFileWatchManager->Modified = true;
                    }
                    break;
                    case FileStatus::FS_ERASED:
                        std::cout << "File erased: " << PATH_TO_WATCH << '\n';
                        break;
                    default:
                        std::cout << "Weird Error\n";

                    }
                }
                );
            }

            if (engine->gFileWatchManager->Modified == true)
            {
                if (engine->gFileWatchManager->HotReloadCooldown <= 5.0f)
                {
                    engine->gFileWatchManager->HotReloadCooldown += engine->Game_Clock.get_fixedDeltaTime();
                }
                else
                {
                    engine->gFileWatchManager->HotReloadCooldown = 0.0f;
                    engine->gFileWatchManager->Modified = false;
                    engine->AssimpManager.ResetHotReloadFlag();
                }
            }
        }
    }
}