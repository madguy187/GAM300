#include "pch.h"
#include "../FileWatchSystem/FileWatchSystem.h"

namespace Eclipse
{
    void FileWatchSystem::Init()
    {
        EclipseFileWatcher watch{ "src/Assets", 2.0f };
        engine->gFileWatchManager = std::make_unique<EclipseFileWatcher>(watch);
    }

    void FileWatchSystem::Update()
    {
        ZoneScopedN("FileWatch System");
        engine->Timer.SetName({ SystemName::FILEWATCH });
        engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

        // I only run in editor state
        if (engine->GetEditorState())
        {
            if (engine->gFileWatchManager->UpdateTimer())
            {
                engine->gFileWatchManager->CheckReloadStatus();

                engine->gFileWatchManager->Start([](std::string PATH_TO_WATCH, FileStatus status) -> void
                {
                    if (!std::filesystem::is_regular_file(std::filesystem::path(PATH_TO_WATCH)) && status != FileStatus::FS_ERASED)
                    {
                        return;
                    }

                    engine->gFileWatchManager->Resolutions(status, PATH_TO_WATCH);
                }
                );
            }
            engine->gFileWatchManager->HardReset();
        }

        engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
        engine->Timer.UpdateTimeContainer(engine->Timer.tracker);

        FrameMark
    }
}