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

        int i = 0;
    }

    void EclipseFileWatcher::Start(const std::function<void(std::string, FileStatus)>& action)
    {
        auto it = paths_.begin();

        while (it != paths_.end())
        {
            if (!std::filesystem::exists(it->first))
            {
                action(it->first, FileStatus::FS_ERASED);
                it = paths_.erase(it);
            }
            else
            {
                it++;
            }

        }

        // Check if a file was created or modified
        for (auto& file : std::filesystem::recursive_directory_iterator(PathToWatch))
        {
            auto CurrentFileLastWriteTime = std::filesystem::last_write_time(file);

            // File creation
            if (Contains(file.path().string()) == false)
            {
                paths_[file.path().string()] = CurrentFileLastWriteTime;
                action(file.path().string(), FileStatus::FS_CREATED);
            }
            else
            {
                if (paths_[file.path().string()] != CurrentFileLastWriteTime)
                {
                    paths_[file.path().string()] = CurrentFileLastWriteTime;
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

    std::chrono::duration<int, std::milli> EclipseFileWatcher::InputTime(unsigned int in)
    {
        unsigned int Caclulated = in * 1000;
        return std::chrono::milliseconds(Caclulated);
    }

    bool EclipseFileWatcher::UpdateTimer()
    {
        if (Timer <= 2.0f)
        {
            Timer += engine->Game_Clock.get_fixedDeltaTime();
            return false;
        }
        else
        {
            Timer = 0.0f;
            return true;
        }
    }

    bool EclipseFileWatcher::CheckBasicTexture(std::string& in)
    {
        if (in.find("src/Assets\\Textures\\") != std::string::npos)
        {
            BasicTextureCounter++;
            return true;
        }

        return false;
    }

    void EclipseFileWatcher::Resolutions(FileStatus status , std::string& PATH_TO_WATCH)
    {
        switch (status)
        {
        case FileStatus::FS_CREATED:
        {
            //std::cout << "File created: " << PATH_TO_WATCH << '\n';
            AssetCounter++;
            CheckBasicTexture(PATH_TO_WATCH);
        }
        break;

        case FileStatus::FS_MODIFIED:
        {
            //std::cout << "File modified: " << PATH_TO_WATCH << '\n';
            AssetCounter++;
            CheckBasicTexture(PATH_TO_WATCH);
        }
        break;

        case FileStatus::FS_ERASED:
        {
            //std::cout << "File erased: " << PATH_TO_WATCH << '\n';
            AssetCounter++;
            CheckBasicTexture(PATH_TO_WATCH);
        }
        break;

        default:
            std::cout << "Weird Error\n";

        }
    }

    void EclipseFileWatcher::HardReset(float in)
    {
        if (AssetCounter)
        {
            engine->AssimpManager.HotReload();
            AssetCounter = 0;
        }

        if (BasicTextureCounter)
        {
            engine->AssimpManager.LoadBasicTextures();
            BasicTextureCounter = 0;
        }
    }
}