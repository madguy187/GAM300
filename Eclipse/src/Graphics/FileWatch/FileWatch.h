#ifndef FILEWATCH_H
#define FILEWATCH_H

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

    enum class ReloadTypes
    {
        RT_NONE = 0,
        RT_FONTS = 1,
        RT_MODELS = 2,
        RT_SCRIPTS = 3,
        RT_SHADERS = 4,
        RT_SKY = 5,
        RT_BASICTEXTURES = 6,
        RT_MATERIALINSTANCE = 7,
        RT_MATERIAL_TETXURES = 8,
        RT_MAXCOUNT
    };

    class EclipseFileWatcher
    {
    private:
        // Counter for Reload types
        unsigned int AssetCounter = 0;
        unsigned int BasicTextureCounter = 0;
        unsigned int Create = 0;
        unsigned int Delete = 0;
        unsigned int MaterialInstanceCounter = 0;
        unsigned int MaterialTexturesCounter = 0;

        bool Modified = false;
        float Delays = 0.0f;
        float Timer = 0.0f;
        float HotReloadCooldown = 0.0f;
        float HardResetTime = 5.0f;
        std::string PathToWatch;
        std::unordered_map<std::string, std::filesystem::file_time_type> paths_;

    public:
        EclipseFileWatcher();
        bool UpdateTimer();
        void HardReset();
        EclipseFileWatcher(std::string path_to_watch, float delay);
        void Start(const std::function<void(std::string, FileStatus)>& action);
        void Resolutions(FileStatus status, std::string& PATH_TO_WATCH);
        void CheckReloadStatus();
        std::chrono::duration<int, std::milli> InputTime(unsigned int in);

    private:
        bool Contains(const std::string& key);
        void ReloadType(std::string& in);
        ReloadTypes CheckFolder(std::string const& inString);
        bool ExcludePath(std::string const& inString);
    };
}
#endif /* FILEWATCH_H */