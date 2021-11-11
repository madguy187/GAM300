#include "pch.h"
#include "EngineCompiler.h"

namespace Eclipse
{
    EngineCompiler::EngineCompiler()
    {

    }

    bool EngineCompiler::OpenFile(std::fstream& FileIn, std::string& Path)
    {
        FileIn.open(Path, std::ios::in | std::ios::binary);

        if (FileIn.fail())
        {
            EDITOR_LOG_WARN(("Failed to open " + Path).c_str());
            return false;
        }

        return true;
    }

    void EngineCompiler::CloseFile(std::fstream& FileIn, std::string& FileName, unsigned int ContainerSize)
    {
        FileIn.close();

        if (ContainerSize != 0)
        {
            EDITOR_LOG_INFO((FileName + " file read and loaded into Engine.").c_str());

            if (FileName == AllNames[0])
            {
                CompilerFlags.set(0, 1);
            }
            else if (FileName == AllNames[1])
            {
                CompilerFlags.set(1, 1);
            }
            else if (FileName == AllNames[2])
            {
                CompilerFlags.set(2, 1);
            }
            else if (FileName == AllNames[3])
            {
                CompilerFlags.set(3, 1);
            }
            else if (FileName == AllNames[4])
            {
                CompilerFlags.set(4, 1);
            }
        }
    }

    void EngineCompiler::LoadGeometry()
    {
        if (OpenFile(GeometryFileRead, GeometryPath) == false)
            return;

        CompilerFlags.set(0, 0);

        //EDITOR_LOG_INFO("Geometry File Open For Reading");

        unsigned int TotalNumberOfModels = 0;
        unsigned int VerticesSize = 0;
        unsigned int IndicesSize = 0;

        // See how many Models
        GeometryFileRead.read(reinterpret_cast<char*>(&TotalNumberOfModels), sizeof(TotalNumberOfModels));

        for (unsigned int i = 0; i < TotalNumberOfModels; i++)
        {
            VerticesSize = 0;
            IndicesSize = 0;

            Mesh B;

            GeometryFileRead.read(reinterpret_cast<char*>(&B), offsetof(Mesh, Vertices));

            GeometryFileRead.read(reinterpret_cast<char*>(&VerticesSize), sizeof(VerticesSize));
            B.Vertices.resize(VerticesSize);
            GeometryFileRead.read(reinterpret_cast<char*>(B.Vertices.data()), sizeof(Vertex) * VerticesSize);

            GeometryFileRead.read(reinterpret_cast<char*>(&IndicesSize), sizeof(IndicesSize));

            B.Indices.resize(IndicesSize);
            GeometryFileRead.read(reinterpret_cast<char*>(B.Indices.data()), sizeof(unsigned int) * IndicesSize);

            std::string name = B.MeshName.data();

            if (B.NoTex == false)
            {
                Mesh NewMesh(B.Vertices, B.Indices, B.MeshName.data(), B.Textures);
                engine->AssimpManager.InsertGeometry(name, NewMesh);
            }
            else
            {
                Mesh NewMesh(B.Vertices, B.Indices, B.Diffuse, B.Specular, B.Ambient, B.NoTex, B.MeshName.data());
                engine->AssimpManager.InsertGeometry(name, NewMesh);
            }

            if (strcmp(name.c_str(), "LightBulb") != 0 && strcmp(name.c_str(), "SpotLight") != 0 && strcmp(name.c_str(), "Inner") != 0 && strcmp(name.c_str(), "Outer") != 0)
            {
                //engine->AssimpManager.InsertMeshName(name);
                engine->AssimpManager.InsertGeometryName(name);
            }
        }

        CloseFile(GeometryFileRead, AllNames[0], TotalNumberOfModels);
    }

    void EngineCompiler::LoadPrefabs()
    {
        if (OpenFile(PrefabsFileRead, PrefabsPath) == false)
            return;

        CompilerFlags.set(1, 0);

        //EDITOR_LOG_INFO("Prefabs File Open For Reading");

        int TotalNumberOfPrefabs = 0;

        // See how many Prefabs
        PrefabsFileRead.read(reinterpret_cast<char*>(&TotalNumberOfPrefabs), sizeof(int));

        for (int i = 0; i < TotalNumberOfPrefabs; i++)
        {
            std::array<char, 128> ParentName;
            PrefabsFileRead.read(reinterpret_cast<char*>(&ParentName), sizeof(ParentName));
            ParentName[ParentName.size() - 1] = '\0';

            int NumberOfSubMeshes = 0;
            PrefabsFileRead.read(reinterpret_cast<char*>(&NumberOfSubMeshes), sizeof(NumberOfSubMeshes));

            for (int j = 0; j < NumberOfSubMeshes; j++)
            {
                std::array<char, 128> MeshName;
                PrefabsFileRead.read(reinterpret_cast<char*>(&MeshName), sizeof(MeshName));
                MeshName[MeshName.size() - 1] = '\0';

                engine->AssimpManager.InsertPrefabs(ParentName.data(), MeshName.data());
            }

            if (strcmp(ParentName.data(), "LightBulb") != 0 && strcmp(ParentName.data(), "SpotLight") != 0 && strcmp(ParentName.data(), "Inner") != 0 && strcmp(ParentName.data(), "Outer") != 0)
            {
                if (NumberOfSubMeshes == 1)
                {
                    engine->AssimpManager.RemoveSubMesh(engine->AssimpManager.Prefabs[ParentName.data()][0]);
                }

                engine->AssimpManager.InsertMeshName(ParentName.data());
            }
        }

        CloseFile(PrefabsFileRead, AllNames[1], TotalNumberOfPrefabs);
    }

    void EngineCompiler::LoadBasicTextures()
    {
        if (OpenFile(TextureFileRead, TexturePath) == false)
            return;

        CompilerFlags.set(2, 0);

        //EDITOR_LOG_INFO("Texture File Open For Reading");

        // Number Of Textures
        int NumberOfBasicTextures = 0;
        TextureFileRead.read(reinterpret_cast<char*>(&NumberOfBasicTextures), sizeof(NumberOfBasicTextures));

        for (int i = 0; i < NumberOfBasicTextures; i++)
        {
            // Texture Name
            std::array<char, 128> TextureName;
            TextureFileRead.read(reinterpret_cast<char*>(&TextureName), sizeof(TextureName));

            // Texture DirecPathtory
            std::array<char, 128> TexturePath_;
            TextureFileRead.read(reinterpret_cast<char*>(&TexturePath_), sizeof(TexturePath_));

            Texture tex(TexturePath_.data());
            Graphics::textures.emplace(TextureName.data(), tex);
        }

        CloseFile(TextureFileRead, AllNames[2], NumberOfBasicTextures);
    }

    void EngineCompiler::LoadModelTextures()
    {
        if (OpenFile(ModelTextureFileRead, ModelTexturePath) == false)
            return;

        CompilerFlags.set(3, 0);

        //EDITOR_LOG_INFO("Texture File Open For Reading");

        // Number Of Textures
        int NumberOfTextures = 0;
        ModelTextureFileRead.read(reinterpret_cast<char*>(&NumberOfTextures), sizeof(NumberOfTextures));

        for (int i = 0; i < NumberOfTextures; i++)
        {
            // Mesh Name
            std::array<char, 128> MeshName;
            ModelTextureFileRead.read(reinterpret_cast<char*>(&MeshName), sizeof(MeshName));

            // Number Of Textures
            int TextureType = 0;
            ModelTextureFileRead.read(reinterpret_cast<char*>(&TextureType), sizeof(TextureType));

            // Texture Directory
            std::array<char, 128> TextureDirectory;
            ModelTextureFileRead.read(reinterpret_cast<char*>(&TextureDirectory), sizeof(TextureDirectory));

            // Texture DirecPathtory
            std::array<char, 128> TexturePath_;
            ModelTextureFileRead.read(reinterpret_cast<char*>(&TexturePath_), sizeof(TexturePath_));

            Texture tex(TextureDirectory.data(), TexturePath_.data(), static_cast<aiTextureType>(TextureType));
            tex.Load(false);
            Graphics::textures.emplace(MeshName.data(), tex);
        }

        CloseFile(ModelTextureFileRead, AllNames[3], NumberOfTextures);
    }

    void EngineCompiler::LoadAnimation()
    {
        if (OpenFile(AnimationFileRead, AnimationPath) == false)
        {
            return;
        }    
        
        CompilerFlags.set(4, 0);
        
        struct AnimationData
        {
            float modelLargestAxis;
            float m_Duration;
            int m_TicksPerSecond;
            std::array<char, 128> fileName;
            std::array<char, 128> modelName;
        };
        
        struct BoneInfoData
        {
            int id;
            glm::mat4 offset;
            std::array<char, 128> name;
        };
        
        struct BoneData
        {
            std::vector<KeyPosition> m_Positions;
            std::vector<KeyRotation> m_Rotations;
            std::vector<KeyScale> m_Scales;
        
            int m_NumPositions;
            int m_NumRotations;
            int m_NumScalings;
        
            int m_ID;
            glm::mat4 m_LocalTransform;
            std::array<char, 128> BoneName;
        };
        
        unsigned int totalAnimation = 0;
        unsigned int boneSize = 0;
        unsigned int boneInfoSize = 0;
        
        unsigned int keyPosSize = 0;
        unsigned int keyRotSize = 0;
        unsigned int keyScaleSize = 0;
        
        unsigned int nodeChildrenSize = 0;
        
        AnimationFileRead.read(reinterpret_cast<char*>(&totalAnimation), sizeof(totalAnimation));
        
        for (unsigned int i = 0; i < totalAnimation; i++)
        {
            boneSize = 0;
            boneInfoSize = 0;
            keyPosSize = 0;
            keyRotSize = 0;
            keyScaleSize = 0;
            nodeChildrenSize = 0;
        
            AnimationData animationdata;
            std::vector<BoneInfoData> boneinfoData;
            std::vector<BoneData> boneData;
        
            std::vector<Bone> newBones;
            std::vector<BoneInfo> newBoneInfos;
            AssimpNodeData rootNode;
       
            AnimationFileRead.read(reinterpret_cast<char*>(&animationdata), (2 * sizeof(float)) + sizeof(int) + 
                sizeof(animationdata.fileName) + sizeof(animationdata.modelName));

            AnimationFileRead.read(reinterpret_cast<char*>(&boneInfoSize), sizeof(boneInfoSize));
            boneinfoData.resize(boneInfoSize);
            AnimationFileRead.read(reinterpret_cast<char*>(boneinfoData.data()), sizeof(BoneInfoData) * boneInfoSize);
        
            // Populate BoneInfo vector
            for (auto& it : boneinfoData)
            {
                BoneInfo newBoneInfo(it.id, it.offset, it.name);
                newBoneInfos.push_back(newBoneInfo);
            }
        
            AnimationFileRead.read(reinterpret_cast<char*>(&boneSize), sizeof(boneSize));
            boneData.resize(boneSize);
        
            for (unsigned int i = 0; i < boneSize; ++i)
            {
                AnimationFileRead.read(reinterpret_cast<char*>(&keyPosSize), sizeof(keyPosSize));
                boneData[i].m_Positions.resize(keyPosSize);
                AnimationFileRead.read(reinterpret_cast<char*>(boneData[i].m_Positions.data()), sizeof(KeyPosition) * keyPosSize);
        
                AnimationFileRead.read(reinterpret_cast<char*>(&keyRotSize), sizeof(keyRotSize));
                boneData[i].m_Rotations.resize(keyRotSize);
                AnimationFileRead.read(reinterpret_cast<char*>(boneData[i].m_Rotations.data()), sizeof(KeyRotation)* keyRotSize);
        
                AnimationFileRead.read(reinterpret_cast<char*>(&keyScaleSize), sizeof(keyScaleSize));
                boneData[i].m_Scales.resize(keyScaleSize);
                AnimationFileRead.read(reinterpret_cast<char*>(boneData[i].m_Scales.data()), sizeof(KeyScale)* keyScaleSize);
        
                AnimationFileRead.read(reinterpret_cast<char*>(&boneData[i].m_NumPositions), sizeof(int));
                AnimationFileRead.read(reinterpret_cast<char*>(&boneData[i].m_NumRotations), sizeof(int));
                AnimationFileRead.read(reinterpret_cast<char*>(&boneData[i].m_NumScalings), sizeof(int));
        
                AnimationFileRead.read(reinterpret_cast<char*>(&boneData[i].m_ID), sizeof(int));
        
                AnimationFileRead.read(reinterpret_cast<char*>(&boneData[i].m_LocalTransform), sizeof(glm::mat4));
                AnimationFileRead.read(reinterpret_cast<char*>(&boneData[i].BoneName), sizeof(boneData[i].BoneName));
        
                //Insert into Bone vector
                Bone newBone(boneData[i].m_Positions, boneData[i].m_Rotations, boneData[i].m_Scales,
                    boneData[i].m_NumPositions, boneData[i].m_NumRotations, boneData[i].m_NumScalings,
                    boneData[i].m_ID, boneData[i].m_LocalTransform, boneData[i].BoneName);
        
                newBones.push_back(newBone);
            }
            
            AnimationFileRead.read(reinterpret_cast<char*>(&rootNode.transformation), sizeof(glm::mat4));
            AnimationFileRead.read(reinterpret_cast<char*>(&rootNode.name), sizeof(rootNode.name));
            AnimationFileRead.read(reinterpret_cast<char*>(&rootNode.childrenCount), sizeof(int));
        
            rootNode.children.resize(rootNode.childrenCount);
            engine->gAnimationManager.RecurseChildren(rootNode, AnimationFileRead);

            AnimationState animationState = engine->gAnimationManager.InitAnimationState(std::string(animationdata.modelName.data()),
                std::string(animationdata.fileName.data()));

            Animation newAnimation(animationdata.modelLargestAxis, animationdata.m_Duration, animationdata.m_TicksPerSecond, animationState,
                animationdata.modelName, newBoneInfos, newBones, rootNode);

            engine->gAnimationManager.InsertAnimation(newAnimation);
        }
        
        CloseFile(AnimationFileRead, AllNames[4], totalAnimation);
    }

    void EngineCompiler::RunCompiler()
    {
        // Please Check this in the future - Darren Sim
#ifdef DEBUG
        {
            system("Compiler-Debug.exe");
            EDITOR_LOG_INFO("Compiler Execution finished.");
        }
#else
        {
            system("Compiler-Release.exe");
            EDITOR_LOG_INFO("Compiler Execution finished.");
        }
#endif 
    }

    bool EngineCompiler::IsGeometryCompiled()
    {
        return CompilerFlags.test(0);
    }

    bool EngineCompiler::IsPrefabsCompiled()
    {
        return CompilerFlags.test(1);
    }
    bool EngineCompiler::IsModelTexturesCompiled()
    {
        return CompilerFlags.test(3);
    }

    bool EngineCompiler::IsBasicTexturesCompiled()
    {
        return CompilerFlags.test(2);
    }

    bool EngineCompiler::IsAnimationCompiled()
    {
        return CompilerFlags.test(4);
    }

    bool EngineCompiler::AreAllCompiled()
    {
        //return IsGeometryCompiled() && IsPrefabsCompiled() && IsModelTexturesCompiled() && IsBasicTexturesCompiled() && IsAnimationCompiled();
        return IsGeometryCompiled() && IsPrefabsCompiled() && IsModelTexturesCompiled() && IsBasicTexturesCompiled();
    }

    void EngineCompiler::LoadCompilers()
    {
        std::thread CompilerWait{ &EngineCompiler::RunCompiler };
        CompilerWait.join();

        //CompilerThreads.emplace("LoadPrefabs", std::make_unique<std::thread>(std::thread{ &EngineCompiler::LoadPrefabs }));
        LoadGeometry();
        LoadPrefabs();
        LoadModelTextures();
        LoadBasicTextures();
        LoadAnimation();
        engine->gPBRManager->gMaterialEditorSettings->gMaterialCompiler.LoadMaterials();
        engine->InputManager->InputCompiler_.Load();
        //CompilerThreads["LoadPrefabs"]->join();
        CompilerThreads.clear();

        if (AreAllCompiled() == true)
        {
            EDITOR_LOG_INFO("All compilers loaded, able to generate Models and Textures.");
        }
        else
        {
            if (IsGeometryCompiled() == true && IsPrefabsCompiled() == true)
            {
                EDITOR_LOG_INFO("Geometry compilers loaded, able to generate Models.");
            }
            else
            {
                if (IsGeometryCompiled() == false)
                {
                    EDITOR_LOG_WARN("Geometry failed to compile, unable to generate models.");
                }

                if (IsPrefabsCompiled() == false)
                {
                    EDITOR_LOG_WARN("Prefabs failed to compile, unable to generate models.");
                }

                if (IsModelTexturesCompiled() == false)
                {
                    EDITOR_LOG_WARN("Model Textures failed to compile, unable to generate Textures for Models.");
                }

                if (IsBasicTexturesCompiled() == false)
                {
                    EDITOR_LOG_WARN("Basic Textures failed to compile, unable to generate Basic Textures.");
                }

                if (IsAnimationCompiled() == false)
                {
                    EDITOR_LOG_WARN("Animation failed to compile, unable to generate animation data.");
                }
            }
        }
    }

    void EngineCompiler::HotReload()
    {
        engine->AssimpManager.ClearAllMeshNames();
        engine->AssimpManager.ClearGeometry();
        engine->AssimpManager.ClearAllPrefabs();
        Graphics::textures.clear();

        std::thread CompilerWait{ &EngineCompiler::RunCompiler };
        CompilerWait.join();

        CompilerThreads.emplace("LoadPrefabs", std::make_unique<std::thread>(std::thread{ &EngineCompiler::LoadPrefabs }));
        LoadGeometry();
        LoadModelTextures();
        LoadBasicTextures();
        CompilerThreads["LoadPrefabs"]->join();
        CompilerThreads.clear();

        if (IsGeometryCompiled() == false)
        {
            EDITOR_LOG_WARN("Geometry failed to load, unable to generate models.");
        }

        if (IsPrefabsCompiled() == false)
        {
            EDITOR_LOG_WARN("Prefabs failed to load, unable to generate models.");
        }

        if (IsModelTexturesCompiled() == false)
        {
            EDITOR_LOG_WARN("Model Textures failed to load.");
        }

        if (IsBasicTexturesCompiled() == false)
        {
            EDITOR_LOG_WARN("Basic Textures failed to load.");
        }
    }

    void EngineCompiler::HotReloadTetxures()
    {
        Graphics::textures.clear();

        std::thread CompilerWait{ &EngineCompiler::RunCompiler };
        CompilerWait.join();

        LoadModelTextures();
        LoadBasicTextures();

        if (IsModelTexturesCompiled() == false)
        {
            EDITOR_LOG_WARN("Model Textures failed to load.");
        }

        if (IsBasicTexturesCompiled() == false)
        {
            EDITOR_LOG_WARN("Basic Textures failed to load.");
        }
    }

    void EngineCompiler::Init()
    {
        LoadCompilers();
    }
}