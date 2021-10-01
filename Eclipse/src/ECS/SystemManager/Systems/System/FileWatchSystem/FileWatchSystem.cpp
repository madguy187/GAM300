#include "pch.h"
#include "../FileWatchSystem/FileWatchSystem.h"
#include "Graphics/FileWatch/FileWatch.h"

namespace Eclipse
{
    void FileWatchSystem::Init()
    {
        EclipseFileWatcher watch{ "src/Assets", engine->gFileWatchManager->InputTime(5) };
        engine->gFileWatchManager = std::make_unique<EclipseFileWatcher>(watch);
    }

    void FileWatchSystem::Update()
    {
        // I only run in editor state
        if (engine->GetEditorState())
        {
            if (engine->gFileWatchManager->UpdateTimer())
            {
                if (engine->gFileWatchManager->AssetCounter != 0 || engine->gFileWatchManager->BasicTextureCounter != 0)
                    return;

                engine->gFileWatchManager->Start([](std::string PATH_TO_WATCH, FileStatus status) -> void
                {
                    if (!std::filesystem::is_regular_file(std::filesystem::path(PATH_TO_WATCH)) && status != FileStatus::FS_ERASED)
                    {
                        return;
                    }

                    // Hot Reloading
                    if (engine->AssimpManager.GetHotReloadFlag())
                        return;

                    engine->gFileWatchManager->Resolutions(status, PATH_TO_WATCH);
                }
                );
            }

            engine->gFileWatchManager->HardReset(engine->gFileWatchManager->HardResetTime);
        }
    }
}