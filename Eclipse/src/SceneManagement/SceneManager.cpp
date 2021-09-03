#include "pch.h"
#include "SceneManager.h"

namespace Eclipse
{
	SceneManager::SceneIndex SceneManager::numOfScene = 0;
	SceneManager::SceneIndex SceneManager::prevScene = 0;
	SceneManager::SceneIndex SceneManager::curScene = 0;
	SceneManager::SceneIndex SceneManager::nextScene = 0;
	bool SceneManager::isQuit = false;
	bool SceneManager::isReload = false;
	std::vector<std::string> SceneManager::sceneList;
	std::unordered_map<std::string, std::string> SceneManager::mapNameToPath;

	SceneManager::SceneManager()
	{
		//RegisterScene();
	}

	void SceneManager::InitStartingScene(const SceneIndex& idx)
	{

	}

	void SceneManager::InitStartingScene(const std::string& name)
	{

	}

	void SceneManager::ProcessScene()
	{
		if (curScene != nextScene)
		{
			Clear();
			if (nextScene == RELOAD)
			{
				nextScene = curScene;
				engine->editorManager->LoadTemp(mapNameToPath[sceneList[curScene]].c_str());
			}
			else if (nextScene != QUIT)
			{
				prevScene = curScene;
				curScene = nextScene;
				engine->editorManager->LoadTemp(mapNameToPath[sceneList[curScene]].c_str());
			}
		}

	}

	SceneManager::SceneIndex SceneManager::RegisterScene(const std::string& path)
	{
		std::filesystem::path finalPath{ path };

		if (!std::filesystem::exists(finalPath))
		{
			ENGINE_CORE_ERROR(false, "Fail to register scene.");
			return -1;
		}

		if (finalPath.has_extension())
		{
			finalPath = finalPath.replace_extension("");
		}

		std::string fileName = finalPath.filename().string();

		if (!CheckSceneAvailable(fileName))
		{
			ENGINE_CORE_WARN(false, "Scene is already registered.");
			return GetSceneIndex(fileName);
		}

		sceneList.push_back(fileName);
		mapNameToPath.insert({ fileName, path });

		return numOfScene++;
	}

	SceneManager::SceneIndex SceneManager::GetPreviousScene()
	{
		return prevScene;
	}

	SceneManager::SceneIndex SceneManager::GetCurrentScene()
	{
		return curScene;
	}

	SceneManager::SceneIndex SceneManager::GetNextScene()
	{
		return nextScene;
	}

	bool SceneManager::CheckSceneAvailable(const std::string& name)
	{
		auto it = mapNameToPath.find(name);

		return it == mapNameToPath.end() ? false : true;
	}

	bool SceneManager::CheckCurrentScene(const SceneIndex& idx)
	{
		return idx == curScene ? true : false;
	}

	bool SceneManager::CheckCurrentScene(const std::string& name)
	{
		return sceneList[curScene] == name ? true : false;
	}

	void SceneManager::DeregisterScene(const SceneIndex& idx)
	{

	}

	void SceneManager::DeregisterScene(const std::string& name)
	{

	}

	SceneManager::SceneIndex SceneManager::GetSceneIndex(const std::string& name)
	{
		SceneIndex count = 0;
		for (auto scn : sceneList)
		{
			if (scn == name)
			{
				return count;
			}
			count++;
		}

		return -1;
	}
	void SceneManager::LoadScene(const SceneIndex& idx)
	{
		if (idx < 0 || idx >= numOfScene)
		{
			ENGINE_CORE_ERROR(false, "Fail to load scene.");
			return;
		}

		nextScene = idx;
	}

	void SceneManager::LoadScene(const std::string& name)
	{
		if (!CheckSceneAvailable(name))
		{
			ENGINE_CORE_ERROR(false, "Fail to load scene.");
			return;
		}
		nextScene = GetSceneIndex(name);
	}

	void SceneManager::LoadNextScene()
	{

	}

	void SceneManager::QuitScene()
	{
		nextScene = QUIT;
	}

	void SceneManager::Clear()
	{
		if (engine->GetEditorState())
		{
			engine->editorManager->Clear();
		}
		engine->world.Clear();
	}

	void SceneManager::ReloadScene()
	{
		nextScene = RELOAD;
	}
}