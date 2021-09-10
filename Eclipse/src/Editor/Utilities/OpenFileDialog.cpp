#include "pch.h"
#include "OpenFileDialog.h"


std::string FileDialog::FileBrowser()
{
	OPENFILENAMEA openFileName;
	char fileName[MAX_PATH] = "";
	wchar_t wFilename[MAX_PATH] = L"";
	mbstowcs(wFilename, fileName, strlen(fileName) + 1);//Plus null
	ZeroMemory(&openFileName, sizeof(openFileName));
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.lpstrFile = fileName;
	openFileName.lpstrFile[0] = '\0';
	openFileName.nMaxFile = MAX_PATH;
	openFileName.lpstrFilter = "All Files\0*.*\0C++ Files\0*.cpp\0Header Files\0*.h\0";
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	std::filesystem::path _originPath{ std::filesystem::current_path() };
	if (GetOpenFileNameA(&openFileName))
	{
		std::filesystem::current_path(_originPath);
		//open file (de-serialize here)
    std::filesystem::path _path{ fileName };
    std::filesystem::path temp{ _path };

    if (std::filesystem::exists(_path))
    {

      std::string name = temp.replace_extension("").filename().string();

      if (SceneManager::CheckSceneAvailable(name))
      {
        SceneManager::LoadScene(name);
      }
      else
      {
        SceneManager::RegisterScene(_path.string());
        SceneManager::LoadScene(name);
      }

      std::string msg = "File ";
      msg += name + "is loaded successfully";
      EDITOR_LOG_INFO(msg.c_str());
    }
		EDITOR_LOG_INFO(_path.string().c_str());
		return _path.string();
	}
	return std::string();
}
std::string FileDialog::SaveFile()
{
  OPENFILENAMEA openFileName;
  char fileName[MAX_PATH] = "";
  wchar_t wFilename[MAX_PATH] = L"";
  mbstowcs(wFilename, fileName, strlen(fileName) + 1);//Plus null
  ZeroMemory(&openFileName, sizeof(openFileName));
  openFileName.lStructSize = sizeof(OPENFILENAME);
  openFileName.lpstrFile = fileName;
  openFileName.lpstrFile[0] = '\0';
  openFileName.nMaxFile = MAX_PATH;
  openFileName.lpstrFilter = "Scene Files\0*.xml\0";
  openFileName.nFilterIndex = 1;
  openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  std::filesystem::path _originPath{ std::filesystem::current_path() };

  if (GetSaveFileNameA(&openFileName))
  {
    std::filesystem::path _path{ fileName };

		std::filesystem::current_path(_originPath);

    engine->szManager.SaveSceneFile(_path.string().c_str());

    SceneManager::RegisterScene(_path.string());

    if (!std::filesystem::exists(_path))
    {
      std::string msg = "File ";
      msg += _path.filename().string() + "is saved successfully";
      EDITOR_LOG_INFO(msg.c_str());
    }
    else
    {
      std::string msg = "File ";
      msg += _path.filename().string() + "is overwritten successfully";
      EDITOR_LOG_INFO(msg.c_str());
    }
		//save file  (serialize here)
		EDITOR_LOG_INFO(_path.string().c_str());
		return  _path.string();
	}
	return std::string();
}
