#include "pch.h"
#include "Graphics/FileWatch/FileWatch.h"

namespace Eclipse
{
    // Default Constructor for me to init
    EclipseFileWatcher::EclipseFileWatcher()
    {

    }

    EclipseFileWatcher::EclipseFileWatcher(std::string path_to_watch, float delay) :
        PathToWatch{ path_to_watch },
        Delays{ delay }
    {
        for (auto& file : std::filesystem::recursive_directory_iterator(path_to_watch))
        {
            if (ExcludePath(file.path().string()))
            {
                paths_[file.path().string()] = std::filesystem::last_write_time(file);
            }

        }
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
        if (Timer <= Delays)
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

    void EclipseFileWatcher::ReloadType(std::string& in)
    {
        switch (CheckFolder(in))
        {
        case ReloadTypes::RT_BASICTEXTURES:
        {
            BasicTextureCounter++;
        }
        break;

        case ReloadTypes::RT_MODELS:
        {
            AssetCounter++;
        }
        break;
        }
    }

    void EclipseFileWatcher::Resolutions(FileStatus status, std::string& PATH_TO_WATCH)
    {
        switch (status)
        {
        case FileStatus::FS_CREATED:
        {
            //std::cout << "File created: " << PATH_TO_WATCH << '\n';
            ReloadType(PATH_TO_WATCH);
            Create++;
        }
        break;

        case FileStatus::FS_MODIFIED:
        {
            //std::cout << "File modified: " << PATH_TO_WATCH << '\n';
            ReloadType(PATH_TO_WATCH);
        }
        break;

        case FileStatus::FS_ERASED:
        {
            //std::cout << "File erased: " << PATH_TO_WATCH << '\n';
            ReloadType(PATH_TO_WATCH);
            Delete++;
        }
        break;

        default:
            std::cout << "Weird Error\n";
        }
    }

    void EclipseFileWatcher::CheckReloadStatus()
    {
        if (AssetCounter != 0 || BasicTextureCounter != 0)
            return;
    }

    void EclipseFileWatcher::HardReset()
    {
        if (AssetCounter)
        {
            EDITOR_LOG_INFO("Preparing to Recompile Models");

            if (Create || Delete)
            {
                system("start Compiler.exe");
                engine->AssimpManager.HotReload();
                engine->AssimpManager.HotReload();
                Create = 0;
                Delete = 0;
            }
            else
            {
                EDITOR_LOG_INFO("Preparing to Recompile Models");
                engine->AssimpManager.HotReload();
            }

            AssetCounter = 0;
            EDITOR_LOG_INFO("All Models Recompiled");
        }

        if (BasicTextureCounter)
        {
            EDITOR_LOG_INFO("Preparing to Recompile Textures");
            Graphics::textures.clear();
            system("start Compiler.exe");
            engine->AssimpManager.LoadTextures();
            engine->AssimpManager.LoadBasicTextures();
            BasicTextureCounter = 0;
            EDITOR_LOG_INFO("All Textures Recompiled");
        }
    }

    ReloadTypes EclipseFileWatcher::CheckFolder(std::string const& inString)
    {
        if (inString.find("src/Assets\\Textures\\") != std::string::npos)
        {
            return ReloadTypes::RT_BASICTEXTURES;
        }

        if (inString.find("src/Assets\\Models\\") != std::string::npos)
        {
            return ReloadTypes::RT_MODELS;
        }
    }

    bool EclipseFileWatcher::ExcludePath(std::string const& inString)
    {
        if (inString.find("src/Assets\\Fonts") != std::string::npos ||
            inString.find("src/Assets\\meshes") != std::string::npos ||
            inString.find("src/Assets\\Scripts") != std::string::npos ||
            inString.find("src/Assets\\Shaders") != std::string::npos ||
            inString.find("src/Assets\\Sounds") != std::string::npos ||
            inString.find("src/Assets\\Test Drag DRop") != std::string::npos)
        {
            //std::cout << "Find" << std::endl;
            return false;
        }

        return true;
    }
}