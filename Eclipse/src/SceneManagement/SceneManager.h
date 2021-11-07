#pragma once
#include "Global.h"

namespace Eclipse
{
	class SceneManager
	{
		using SceneIndex = int;
		static std::vector<std::string> sceneList;
		static std::unordered_map<std::string, std::string> mapNameToPath;
		static SceneIndex numOfScene;
		static SceneIndex prevScene;
		static SceneIndex curScene;
		static SceneIndex nextScene;
		static SceneIndex tempScene;
		static bool isReload;
		static bool isQuit;

	public:
		static const char*  EMPTY_SCENE_NAME;
		const static SceneIndex EMPTY = INT_MIN;
		const static SceneIndex QUIT = INT_MAX;
		const static SceneIndex RELOAD = INT_MAX - 1;

		static void Initialize();

		static void ProcessScene();

		/*static void InitStartingScene(const SceneIndex& idx);
		
		static void InitStartingScene(const std::string& name);*/

		static SceneIndex RegisterScene(const std::string& path);

		static SceneIndex RegisterTempScene(const std::string& path);

		static SceneIndex GetCurrentScene();

		static SceneIndex GetPreviousScene();

		static SceneIndex GetNextScene();

		static std::string GetCurrentSceneName();

		static std::string GetPreviousSceneName();

		static std::string GetNextSceneSceneName();

		static std::string GetScenePath(const std::string& name);

		static bool CheckSceneAvailable(const std::string& name);

		static bool CheckCurrentScene(const SceneIndex& idx);

		static bool CheckCurrentScene(const std::string& name);

		static void DeregisterScene(const SceneIndex& idx);

		static void DeregisterTempScene();

		static void DeregisterCurrentScene();

		static void DeregisterScene(const std::string& name);

		static SceneIndex GetSceneIndex(const std::string& name);

		static void LoadScene(const SceneIndex& idx);

		static void LoadScene(const std::string& name);

		static void LoadNextScene();

		static void QuitScene();

		static void Clear();

		static void ReloadScene();

		static void NewScene();
	};
}