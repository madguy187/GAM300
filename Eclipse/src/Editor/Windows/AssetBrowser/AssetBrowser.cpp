#include "pch.h"
#include "AssetBrowser.h"
#include "Editor/Windows/Inspector/Inspector.h"
#include "Editor/Windows/MeshEditor/MeshEditor.h"
#include "Editor/Windows/MaterialEditor/MaterialEditor.h"

namespace Eclipse
{
    void AssetBrowserWindow::Update()
    {
        if (IsVisible)
            ECGui::DrawMainWindow<void()>(WindowName, std::bind(&AssetBrowserWindow::DrawImpl, this));
    }

    void AssetBrowserWindow::Init()
    {
        Type = EditorWindowType::EWT_ASSETBROWSER;
        WindowName = "AssetBrowser " ICON_MDI_FILE_IMAGE;
        memset(searchItemBuffer, 0, 128);
        memset(searchFolderBuffer, 0, 128);
        buttonSize = { thumbnailSize,thumbnailSize };
        allExtensions = { {"cs"},{"png"},{"txt"},{"mat"},{"ITEM"},{"prefab"},{"wav"},{"dds"} };
        ScanAll();
    }

    void AssetBrowserWindow::Unload()
    {
    }

    AssetBrowserWindow::AssetBrowserWindow()
        :CurrentDir(AllDir), AllDir(AssetPath), padding(16.0f), thumbnailSize(120.0f)
    {
    }

    void AssetBrowserWindow::DrawImpl()
    {
        //temp render components test phase

        ECGui::SetColumns(2);

        ECGui::SetColumnOffset(1, 150);

        //left side
        ECGui::DrawChildWindow<void()>({ "##folders_common" }, std::bind(&AssetBrowserWindow::LeftFolderHierarchy, this));

        ECGui::NextColumn();

        //right side

        ECGui::DrawChildWindow<void()>({ "##directory_structure", ImVec2(0, ECGui::GetWindowHeight() - 65) }, std::bind(&AssetBrowserWindow::RightFoldersAndItems, this));


        if (refresh == true)
        {
            ScanAll();
            EDITOR_LOG_INFO("Asset Browser Refreshed!");
            refresh = false;
        }
    }
    template <typename T>
    void AssetBrowserWindow::CreateTreeNode(std::string name, std::function<T> function)
    {
        if (ECGui::BeginTreeNode(name.c_str()))
        {
            function();
            ECGui::EndTreeNode();
        }
    }

    void AssetBrowserWindow::LeftFolderHierarchy()
    {

        SearchFolders();

        if (ECGui::CreateCollapsingHeader("Content"))
        {
            if (!searchFolderMode)
            {
                LeftFolders();

                ResetTreeNodeOpen = false;
            }
            else
            {
                ResetTreeNodeOpen = true;

                LeftSearchedFolders();
            }

        }

    }

    void AssetBrowserWindow::LeftFolders()
    {
        for (auto& dirEntry : std::filesystem::directory_iterator(AllDir))
        {
            if (ResetTreeNodeOpen)
            {
                ECGui::SetNextItemOpen(false);
            }

            const auto& path = dirEntry.path();

            std::string fileNameString = path.filename().string();

            if (dirEntry.is_directory())
            {

                if (ECGui::BeginTreeNode(fileNameString.c_str()))
                {
                    //setting the subDir to Current Dir
                    NextDir = AllDir;

                    //setting its nxt path so that the tree node
                    //shows all paths the current path hold afterwards

                    NextPath(NextDir, path);

                    //checking if the dir does not exist 
                    if (!exists(NextDir))
                    {
                        NextDir = AllDir;
                    }

                    //engine->editorManager->DragAndDropInst_.AssetBrowerFilesAndFoldersTarget("ITEM", paths, AssetPath.string(), dirEntry, refresh, pathMap, CopyFilesAndFolder);

                    for (auto& secondEntry : std::filesystem::recursive_directory_iterator(NextDir))
                    {
                        if (secondEntry.is_directory())
                        {
                            const auto& path2 = secondEntry.path();

                            std::string fileNameString2 = path2.filename().string();

                            if (ECGui::BeginTreeNode(fileNameString2.c_str()))
                            {
                                if (/*ECGui::IsMouseDoubleClicked(0) &&*/ECGui::IsItemClicked(0))
                                {
                                    NextDir = path2;

                                    CurrentDir = NextDir;

                                    if (!exists(CurrentDir))
                                    {
                                        CurrentDir = AllDir;
                                    }

                                    jumpDir = true;
                                }

                                ECGui::EndTreeNode();
                            }
                            //engine->editorManager->DragAndDropInst_.AssetBrowerFilesAndFoldersTarget("ITEM", paths, AssetPath.string(), secondEntry, refresh, pathMap, CopyFilesAndFolder);
                        }
                    }
                    ECGui::EndTreeNode();
                }

                //engine->editorManager->DragAndDropInst_.AssetBrowerFilesAndFoldersTarget("ITEM", paths, AssetPath.string(), dirEntry, refresh, pathMap, CopyFilesAndFolder);

                if (!jumpDir && /*ECGui::IsMouseDoubleClicked(0) &&*/ ECGui::IsItemClicked(0))
                {
                    NextPath(CurrentDir, path);
                }

                if (jumpDir)
                {
                    jumpDir = false;
                }
            }

        }
    }

    void AssetBrowserWindow::LeftSearchedFolders()
    {
        for (auto& pair : FolderMap)
        {
            ECGui::SetNextItemOpen(true);

            if (ECGui::BeginTreeNode((pair.first.filename().string().c_str())))
            {
                for (auto& pair2 : pair.second)
                {
                    std::string temp = pair2.filename().string().c_str();
                    if (!BuffIsEmpty(searchFolderBuffer) && temp.find(searchFolderBuffer) != std::string::npos)
                    {
                        //ECGui::SetNextItemOpen(true, ECGuiCond_Once);
                        if (ECGui::BeginTreeNode(temp.c_str()))
                        {
                            if (ECGui::IsMouseDoubleClicked(0) && ECGui::IsItemClicked(0))
                            {
                                NextDir = pair2;

                                CurrentDir = NextDir;

                                if (!exists(CurrentDir))
                                {
                                    CurrentDir = AllDir;
                                }
                            }
                            ECGui::EndTreeNode();
                        }
                    }
                }
                ECGui::EndTreeNode();
            }

        }
    }

    void AssetBrowserWindow::RightFoldersAndItems()
    {

        //top path 
        ECGui::DrawChildWindow<void()>({ "##top_bar", ImVec2(0, 70) }, std::bind(&AssetBrowserWindow::PathAndSearches, this));

        ECGui::InsertHorizontalLineSeperator();

        //folders and items
        std::string thisDir = CurrentDir.string();
        ECGui::DrawChildWindow<void()>({ "Scrolling" }, std::bind(&AssetBrowserWindow::FoldersAndItems, this));
        engine->editorManager->DragAndDropInst_.StringPayloadTarget("Entity", thisDir,
            "Prefab generated", PayloadTargetType::PTT_ASSETS);

    }

    void AssetBrowserWindow::FoldersAndItems()
    {

        //rightside with told path

        cellSize = thumbnailSize + padding;

        panelWidth = ECGui::GetContentRegionAvail().x;

        columnCount = (int)(panelWidth / cellSize);

        if (columnCount < 1)
        {
            columnCount = 1;
        }

        ECGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ECGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.35f));

        if (ECGui::BeginPopupContextWindow(0, 1, false))
        {
            //for pop ups
            ECGui::EndPopup();
        }

        ECGui::SetColumns(columnCount, 0, false);


        static ImVec2 button_sz(thumbnailSize, thumbnailSize);

        //ECGui::DragFloat2("size", (float*)&button_sz, 0.5f, 1.0f, 200.0f, "%.0f");

        if (!searchItemMode)
        {
            ItemsAndFolders();
        }
        else
        {
            if (CurrentDir == "src\\Assets")
            {
                SearchInBaseFolder();
            }
            else
            {
                SearchInFolders();
            }
        }
        ECGui::SetColumns(1);

        ECGui::PopStyleColor(2);


    }

    void AssetBrowserWindow::PathAndSearches()
    {
        Path();

        SearchItems();
    }

    void AssetBrowserWindow::Path()
    {
        std::string source = { "src" };

        int id = 0, newPathId = -1;

        for (auto& it : this->CurrentDir)
        {
            ECGui::PushID(id);
            if (id > 0)
            {
                ECGui::InsertSameLine();

                ECGui::DrawTextWidget("/", "");

                ECGui::InsertSameLine();
            }
            if (it.string() != source)
            {
                if (ECGui::SmallButton((it.u8string()).c_str()))
                {
                    newPathId = id;
                }
            }

            ECGui::PopID();

            ++id;
        }

        if (newPathId >= 0)
        {
            int i = 0;

            std::filesystem::path newPath;

            for (auto& sec : this->CurrentDir)
            {
                if (i++ > newPathId)
                {
                    break;
                }

                newPath /= sec;
            }

            this->CurrentDir = newPath;
        }
    }

    void AssetBrowserWindow::ItemsAndFolders()
    {
        for (auto& dirEntry : std::filesystem::directory_iterator(CurrentDir))
        {
            const auto& path = dirEntry.path();

            auto relativePath = std::filesystem::relative(path, AssetPath);

            std::string fileNameString = relativePath.filename().string();

            ECGui::PushID(fileNameString.c_str());

            int icon = dirEntry.is_directory() ? engine->editorManager->FolderIcon_ : engine->editorManager->spriteIcon_;

            ECGui::ImageButton((void*)(size_t)icon, buttonSize, { 1,0 }, { 2,1 });

            std::string temp;
            temp = relativePath.filename().string().c_str();

            DragAndDropWrapper(temp,relativePath.string(),dirEntry);

            //engine->editorManager->DragAndDropInst_.AssetBrowerFilesAndFoldersTarget("ITEM", paths, AssetPath.string(), dirEntry, refresh, pathMap, CopyFilesAndFolder);

            static bool test = false;
            if (ECGui::IsMouseDoubleClicked(0) && ECGui::IsItemClicked(0) && ECGui::IsItemHovered())
            {
                if (dirEntry.is_directory())
                {
                    NextPath(CurrentDir, path);

                }
                else
                {
                    std::string temp1;
                    temp1 = GetFileName(relativePath.filename().string().c_str());
                    EDITOR_LOG_INFO(temp1.c_str());
                }
            }

            if (ECGui::IsItemHovered())
            {
                ECGui::SetToolTip(relativePath.string().c_str());
            }

            if (found && searchItemsLowerCase == LowerCase(relativePath.filename().string().c_str()))
            {
                ImDrawList* draw_list = ECGui::GetWindowDrawList();

                ECGui::DrawTextWidget(fileNameString.c_str(), "");

                draw_list->AddRect(ECGui::GetItemRectMin(), ECGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
            }
            else
            {
                ECGui::DrawTextWidget(fileNameString.c_str(), "");
            }

            ECGui::NextColumn();
            ECGui::PopID();
        }
    }

    void AssetBrowserWindow::BackToParentPath(std::filesystem::path& CurrentPath)
    {
        CurrentPath = CurrentPath.parent_path();
    }

    void AssetBrowserWindow::NextPath(std::filesystem::path& CurrentPath, std::filesystem::path pathsofItem)
    {
        CurrentPath /= pathsofItem.filename();

        if (!exists(CurrentPath))
        {
            CurrentPath = pathsofItem;
        }
    }

    void AssetBrowserWindow::AddToPathMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> subDirItems)
    {
        if (pathMap.find(dirEntry) != pathMap.end())
        {
            pathMap.insert_or_assign(dirEntry, subDirItems);
        }
        else
        {
            pathMap.emplace(dirEntry, subDirItems);
        }

    }

    void AssetBrowserWindow::AddToFolderMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> Folder)
    {
        if (FolderMap.find(dirEntry) != FolderMap.end())
        {
            FolderMap.insert_or_assign(dirEntry, Folder);
        }
        else
        {
            FolderMap.emplace(dirEntry, Folder);
        }

    }

    bool AssetBrowserWindow::ExistInVector(std::string Path, std::vector<std::string> container)
    {
        if (std::find(container.begin(), container.end(), Path) != container.end())
        {
            return true;
        }
        return false;
    }

    void AssetBrowserWindow::GetIndex(std::vector<std::string> container, std::string key, int& pos)
    {
        auto it = std::find(container.begin(), container.end(), key);

        // If element was found
        if (it != container.end())
        {
            pos = static_cast<int>(it - container.begin());
        }
    }

    void AssetBrowserWindow::ScanAll()
    {
        std::vector<std::filesystem::path> tempSubDirFolders;

        std::vector<std::filesystem::path> tempSubDirItems;

        for (auto& dirEntry : std::filesystem::directory_iterator(AllDir))
        {
            const auto& path = dirEntry.path();

            auto relativePath = relative(path, AssetPath);

            std::string fileNameString = relativePath.filename().string();

            if (dirEntry.is_directory())
            {
                NextDir = AllDir;

                NextPath(NextDir, path);

                if (!exists(NextDir))
                {
                    NextDir = AllDir;
                }

                for (auto& secondEntry : std::filesystem::recursive_directory_iterator(NextDir))
                {
                    if (secondEntry.is_directory())
                    {
                        tempSubDirFolders.push_back(secondEntry);
                    }

                    if (!ExistInVector(secondEntry.path().string(), subDirItemsPath))
                    {
                        subDirItemsPath.push_back(secondEntry.path().string());
                    }

                    tempSubDirItems.push_back(secondEntry);
                }

            }


            AddToFolderMap(dirEntry, tempSubDirFolders);

            AddToPathMap(dirEntry, tempSubDirItems);

            tempSubDirFolders.clear();

            tempSubDirItems.clear();
        }

    }

    void AssetBrowserWindow::SearchItems()
    {
        if (ECGui::DrawInputTextHintWidget("", "Search..." ICON_MDI_MAGNIFY, searchItemBuffer, 128))
        {
            if (BuffIsEmpty(searchItemBuffer))
            {
                searchItemMode = false;
            }
            else
            {
                searchItemMode = true;
            }

            //when theres nothing in the buffer

            // this is the string i am searching in the directories
            //searchItemsLowerCase = LowerCase(searchItemBuffer);
            searchItemsLowerCase = searchItemBuffer;

        }

        ECGui::InsertSameLine();

        if (ECGui::ButtonBool("Clear" ICON_MDI_NUKE, { 70,20 }))
        {
            if (BuffIsEmpty(searchItemBuffer))
            {
                searchItemMode = false;
                EDITOR_LOG_WARN("Cleared Failed: Buffer Empty");
            }
            else
            {
                memset(searchItemBuffer, 0, 128);
                searchItemMode = false;
                EDITOR_LOG_INFO("Item Search Buffer Cleared");
            }
        }

        if (ECGui::ButtonBool("Refresh" ICON_MDI_REFRESH, { 70,20 }))
        {
            refresh = true;
        }
        ECGui::InsertSameLine();

        ECGui::CheckBoxBool("Copy Mode", &CopyFilesAndFolder);

        ECGui::InsertSameLine();

        if (ECGui::IsItemHovered())
        {
            ECGui::SetToolTip("When ticked copy mode ellse moving mode.");
        }

        //left side search for all files & folders in that current dir
        //right side search for all folders
    }

    void AssetBrowserWindow::SearchFolders()
    {
        if (ECGui::DrawInputTextHintWidget("", "Search..." ICON_MDI_MAGNIFY, searchFolderBuffer, 128))
        {
            if (BuffIsEmpty(searchFolderBuffer))
            {
                searchFolderMode = false;
            }
            else
            {
                searchFolderMode = true;
            }

            //when theres nothing in the buffer

            // this is the string i am searching in the directories
            searchFoldersLowerCase = LowerCase(searchFolderBuffer);

        }
    }

    void AssetBrowserWindow::SearchInBaseFolder()
    {
        MainSearchLogic(subDirItemsPath);
    }

    void AssetBrowserWindow::SearchInFolders()
    {
        for (auto const& pair : pathMap)
        {
            if (pair.first == CurrentDir)
            {
                MainSearchLogic(pair);
            }
        }
    }

    void AssetBrowserWindow::MainSearchLogic(std::map<std::filesystem::path, std::vector<std::filesystem::path>>::value_type Key)
    {
        for (auto const& pair2 : Key.second)
        {
            folderString = pair2.filename().string().c_str();

            if (searchItemsLowerCase == folderString)
            {
                //found
                found = true;
                //CurrentDir = pair.first;

                if (BuffIsEmpty(searchItemBuffer))
                {
                    found = false;
                }
            }
            else
            {
                if (folderString.find(searchItemsLowerCase) != std::string::npos)
                {
                    //CurrentDir = pair.first;
                    found = true;
                }
            }

            std::string nameString = pair2.filename().string();

            const auto& path = pair2;

            auto relativePath = std::filesystem::relative(path, AssetPath);

            std::string fileNameString = relativePath.filename().string();


            if (found && nameString.find(searchItemsLowerCase) != std::string::npos)
            {
                int icon = std::filesystem::is_directory(pair2) ? engine->editorManager->FolderIcon_ : engine->editorManager->spriteIcon_;
                ECGui::ImageButton((void*)(size_t)(intptr_t)icon,
                    buttonSize,
                    { 1,0 },
                    { 2,1 });

                if (ECGui::IsMouseDoubleClicked(0) && ECGui::IsItemClicked(0) && ECGui::IsItemHovered())
                {
                    //files do something ?
                }

                if (ECGui::IsItemHovered())
                {
                    ECGui::SetToolTip(relativePath.string().c_str());
                }

                ImDrawList* draw_list = ECGui::GetWindowDrawList();

                ECGui::DrawTextWrappedWidget(fileNameString.c_str(), "");

                draw_list->AddRect(ECGui::GetItemRectMin(), ECGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));

                ECGui::NextColumn();
            }

        }
    }

    void AssetBrowserWindow::MainSearchLogic(std::vector<std::string> subDirItemsPaths)
    {

        //path that can be used in the future to get the item
        std::filesystem::path tempPath;

        for (auto const& pair2 : subDirItemsPaths)
        {
            tempPath = pair2;


            if (!std::filesystem::is_directory(tempPath))
            {
                std::string tempDir;
                findRootPath(pair2, tempDir);
                if (!BuffIsEmpty(searchItemBuffer) && tempPath.string().find(searchItemsLowerCase) != std::string::npos)
                {
                    auto relativePath = std::filesystem::relative(tempPath, AssetPath);

                    int icon = std::filesystem::is_directory(tempPath) ? engine->editorManager->FolderIcon_ : engine->editorManager->spriteIcon_;
                    ECGui::ImageButton((void*)(intptr_t)icon,
                        buttonSize,
                        { 1,0 },
                        { 2,1 });

                    if (ECGui::IsMouseDoubleClicked(0) && ECGui::IsItemClicked(0) && ECGui::IsItemHovered())
                    {
                        this->CurrentDir = std::filesystem::path(tempDir);
                        memset(searchItemBuffer, 0, 255);
                        searchItemMode = false;
                    }

                    if (ECGui::IsItemHovered())
                    {
                        ECGui::SetToolTip(pair2.c_str());
                    }

                    ImDrawList* draw_list = ECGui::GetWindowDrawList();

                    ECGui::DrawTextWrappedWidget(tempPath.filename().string().c_str(), "");

                    draw_list->AddRect(ECGui::GetItemRectMin(), ECGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));

                    ECGui::NextColumn();
                }
            }

        }
    }

    std::string AssetBrowserWindow::LowerCase(const char* buffer)
    {
        //std::string lowerCaseString;

        std::string bufferString(buffer);
        std::for_each(bufferString.begin(), bufferString.end(), [](char& c)
            {
                c = static_cast<char>(::tolower(c));
            });

        size_t lastdot = bufferString.find_last_of(".");

        if (lastdot == std::string::npos)
        {
            return bufferString;
        }
        return bufferString.substr(0, lastdot);
    }

    std::string AssetBrowserWindow::GetFileName(const char* buffer)
    {

        std::string fileName(buffer);

        size_t lastdot = fileName.find_last_of(".");

        if (lastdot == std::string::npos)
        {
            return fileName;
        }

        return fileName.substr(0, lastdot);
    }

    bool AssetBrowserWindow::BuffIsEmpty(const char* buffer)
    {
        if (strlen(buffer) == 0)
        {
            return true;
        }

        return false;
    }

    void AssetBrowserWindow::findRootPath(std::string inputPath, std::string& outputPath)
    {
        std::string temp = outputPath;
        std::string assetPath = "src\\Assets";
        std::string outPut;
        const size_t last_slash_idx = inputPath.rfind('\\');
        if (std::string::npos != last_slash_idx)
        {
            temp = inputPath.substr(0, last_slash_idx);
        }
        outputPath = temp;
        if ((temp.compare(assetPath) != 0))
        {
            findRootPath(outputPath, outPut);
        }
        else
        {
            return;
        }
    }

    std::map<std::filesystem::path, std::vector<std::filesystem::path>> AssetBrowserWindow::getFolderMap()
    {
        return FolderMap;
    }

    std::map<std::filesystem::path, std::vector<std::filesystem::path>> AssetBrowserWindow::getPathMap()
    {
        return pathMap;
    }

    void AssetBrowserWindow::DragAndDropWrapper(std::string filename, std::string relativePath_, std::filesystem::path dirEntry)
    {
        std::string temp;
        switch (str2int(filename.substr(filename.find_last_of(".") + 1).c_str()))
        {
        case str2int("png"):
            engine->editorManager->DragAndDropInst_.StringPayloadSource("png", relativePath_);
            break;
        case str2int("cs"):
            engine->editorManager->DragAndDropInst_.StringPayloadSource("cs", relativePath_);
            break;
        case str2int("txt"):
            engine->editorManager->DragAndDropInst_.StringPayloadSource("txt", relativePath_);
            break;
        case str2int("wav"):
            engine->editorManager->DragAndDropInst_.StringPayloadSource("wav", "src\\Assets\\" + relativePath_);
            break;
        case str2int("prefab"):
            
            temp = "src\\Assets\\" + relativePath_;

            if (ECGui::IsMouseDoubleClicked(0) && ECGui::IsItemClicked(0) && ECGui::IsItemHovered())
            {
                auto* meshEditor = engine->editorManager->GetEditorWindow<MeshEditorWindow>();

                if (!meshEditor->IsVisible)
                {
                    Entity ent = MAX_ENTITY;
                    engine->szManager.LoadPrefabFile(ent, temp.c_str(), true);
                    meshEditor->SetMeshID(ent);
                    meshEditor->SetPath(temp);
                    engine->editorManager->SetMeshEditorActive(true);
                    meshEditor->IsVisible = true;
                }
            }

            engine->editorManager->DragAndDropInst_.StringPayloadSource("prefab", temp);
            //engine->editorManager->DragAndDropInst_.StringPayloadSource("prefab", "src\\Assets\\" + relativePath.string());
            break;
        case str2int("mat"):

            temp = "src\\Assets\\" + relativePath_;

            if (ECGui::IsMouseDoubleClicked(0) && ECGui::IsItemClicked(0) && ECGui::IsItemHovered())
            {
                size_t lastindex = filename.find_last_of(".");
                std::string tempName = filename.substr(0, lastindex);

                auto* MaterialEditor = engine->editorManager->GetEditorWindow<MaterialEditorWindow>();

                if (!MaterialEditor->IsVisible)
                {
                    engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial = *engine->gPBRManager->AllMaterialInstances[tempName];
                    MaterialEditor->IsVisible = true;
                }
                else
                {
                    engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial = *engine->gPBRManager->AllMaterialInstances[tempName];
                }
            }

            engine->editorManager->DragAndDropInst_.StringPayloadSource("mat", relativePath_);
            break;
        case str2int("dds"):
            engine->editorManager->DragAndDropInst_.StringPayloadSource("dds", relativePath_);
            break;
        default:
            engine->editorManager->DragAndDropInst_.StringPayloadSource("ITEM", relativePath_);
            break;
        }

        //relativePath.string()
        //// GetFileName(relativePath.filename().string().c_str())
        std::string parentPath = std::filesystem::path(dirEntry).string();
        parentPath = parentPath.substr(0, parentPath.find_last_of("/\\"));

        std::string materialFolder = AssetPath.string() + "\\Materials";

        if (parentPath != AssetPath.string() && (parentPath != materialFolder))
        {
            for (size_t i = 0; i < allExtensions.size(); ++i)
            {
                engine->editorManager->DragAndDropInst_.AssetBrowerFilesAndFoldersTarget(allExtensions[i].c_str(), paths, AssetPath.string(), dirEntry, refresh, pathMap, CopyFilesAndFolder);
            }
        }

    }
}
