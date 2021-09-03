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
		static bool isReload;
		static bool isQuit;

		const static SceneIndex QUIT = INT_MAX;
		const static SceneIndex RELOAD = INT_MAX - 1;

	public:
		SceneManager();

		static void ProcessScene();

		static void InitStartingScene(const SceneIndex& idx);
		
		static void InitStartingScene(const std::string& name);

		static SceneIndex RegisterScene(const std::string& path);

		static SceneIndex GetCurrentScene();

		static SceneIndex GetPreviousScene();

		static SceneIndex GetNextScene();

		static bool CheckSceneAvailable(const std::string& name);

		static bool CheckCurrentScene(const SceneIndex& idx);

		static bool CheckCurrentScene(const std::string& name);

		static void DeregisterScene(const SceneIndex& idx);

		static void DeregisterScene(const std::string& name);

		static SceneIndex GetSceneIndex(const std::string& name);

		static void LoadScene(const SceneIndex& idx);

		static void LoadScene(const std::string& name);

		static void LoadNextScene();

		static void QuitScene();

		static void Clear();

		static void ReloadScene();
	};
}