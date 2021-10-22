#include "pch.h"
#include "SceneManager.h"
#include "../ECS/SystemManager/Systems/System/CameraSystem/CameraSystem.h"

namespace Eclipse
{
	SceneManager::SceneIndex SceneManager::numOfScene = 0;
	SceneManager::SceneIndex SceneManager::prevScene = SceneManager::EMPTY;
	SceneManager::SceneIndex SceneManager::curScene = SceneManager::EMPTY;
	SceneManager::SceneIndex SceneManager::nextScene = SceneManager::EMPTY;
	SceneManager::SceneIndex SceneManager::tempScene = SceneManager::EMPTY;
	bool SceneManager::isQuit = false;
	bool SceneManager::isReload = false;
	std::vector<std::string> SceneManager::sceneList;
	std::unordered_map<std::string, std::string> SceneManager::mapNameToPath;

	void SceneManager::Initialize()
	{
		engine->pfManager.PostUpdate();
	}

	void SceneManager::ProcessScene()
	{
		if (curScene != nextScene)
		{
			Clear();
			if (nextScene == RELOAD)
			{
				nextScene = curScene;
				if (curScene != EMPTY)
				{
					engine->szManager.LoadSceneFile(mapNameToPath[sceneList[curScene]].c_str());
				}
			}
			else if (nextScene == EMPTY)
			{
				curScene = nextScene;
				return;
			}
			else if (nextScene != QUIT)
			{
				prevScene = curScene;
				curScene = nextScene;
				engine->szManager.LoadSceneFile(mapNameToPath[sceneList[curScene]].c_str());
				Initialize();
			}
			else if (nextScene == QUIT)
			{

			}
		}

	}

	SceneManager::SceneIndex SceneManager::RegisterScene(const std::string& path)
	{
		std::filesystem::path finalPath{ path };

		if (!std::filesystem::exists(finalPath))
		{
			EDITOR_LOG_WARN(false, "Fail to register scene.");
			return -1;
		}

		if (finalPath.has_extension())
		{
			finalPath = finalPath.replace_extension("");
		}

		std::string fileName = finalPath.filename().string();

		if (CheckSceneAvailable(fileName))
		{
			std::string msg = "Scene is already registered.";
			EDITOR_LOG_WARN(msg.c_str());
			return GetSceneIndex(fileName);
		}

		sceneList.push_back(fileName);
		mapNameToPath.insert({ fileName, path });

		return numOfScene++;
	}

	SceneManager::SceneIndex SceneManager::RegisterTempScene(const std::string& path)
	{
		if (tempScene == EMPTY)
		{
			tempScene = RegisterTempScene(path);
			EDITOR_LOG_INFO("Temp.scn is created.")
		}
		else
		{
			EDITOR_LOG_WARN("Creation of Temp.scn failed. Temp.scn is already available.")
		}
		return tempScene;
	}

	void SceneManager::DeregisterTempScene()
	{
		if (tempScene != EMPTY)
		{
			DeregisterScene(tempScene);
			EDITOR_LOG_INFO("Temp.scn is deregistered.")
		}
		else
		{
			EDITOR_LOG_WARN("Deregisteration of Temp.scn failed. Temp.scn is already deregistered.")
		}
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

	std::string SceneManager::GetCurrentSceneName()
	{
		if (curScene == EMPTY)
		{
			return {};
		}

		return sceneList[curScene];
	}

	std::string SceneManager::GetPreviousSceneName()
	{
		if (prevScene == EMPTY)
		{
			return {};
		}

		return sceneList[prevScene];
	}

	std::string SceneManager::GetNextSceneSceneName()
	{
		if (nextScene == EMPTY || nextScene == QUIT || 
			nextScene == RELOAD)
		{
			return {};
		}

		return sceneList[nextScene];
	}

	std::string SceneManager::GetScenePath(const std::string& name)
	{
		return mapNameToPath[name];
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
		if (curScene == EMPTY)
		{
			return false;
		}

		return sceneList[curScene] == name ? true : false;
	}

	void SceneManager::DeregisterScene(const SceneIndex& idx)
	{
		if (idx == EMPTY || idx == RELOAD || idx == QUIT)
		{
			return;
		}

		std::string deleting = sceneList[idx];

		sceneList.erase(std::next(sceneList.begin(), idx));
		numOfScene--;
		mapNameToPath.erase(deleting);
	}

	void SceneManager::DeregisterScene(const std::string& name)
	{
		(void)name;
	}

	void SceneManager::DeregisterCurrentScene()
	{
		DeregisterScene(curScene);
		curScene = EMPTY;
		nextScene = EMPTY;
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
		if (idx != QUIT && idx != RELOAD && idx != EMPTY &&
			(idx < 0 || idx >= numOfScene))
		{
			EDITOR_LOG_WARN(false, "Fail to load scene.");
			return;
		}

		nextScene = idx;
	}

	void SceneManager::LoadScene(const std::string& name)
	{
		if (!CheckSceneAvailable(name))
		{
			EDITOR_LOG_WARN(false, "Fail to load scene.");
			return;
		}

		nextScene = GetSceneIndex(name);

		if (nextScene == curScene)
		{
			ReloadScene();
		}
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
			engine->gPhysics.CleanupScene();
			engine->editorManager->Clear();
			engine->gPicker.ResetScene();
			CommandHistory::Clear();
		}
		//engine->AssimpManager.ClearContainer();
		engine->world.Clear();
		engine->gCamera.ResetScene();
		engine->gCullingManager->Clear();
		engine->gDynamicAABBTree.ResetTree();
	}

	void SceneManager::ReloadScene()
	{
		nextScene = RELOAD;
	}

	void SceneManager::NewScene()
	{
		nextScene = EMPTY;
		if (curScene == nextScene)
		{
			ReloadScene();
		}
		std::string msg("New Scene is openned.");
		EDITOR_LOG_INFO(msg.c_str())
	}
}