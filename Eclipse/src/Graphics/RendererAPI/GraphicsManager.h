#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "ECS/ComponentManager/Components/MeshComponent.h"
#include "AssimpModel/AssimpModel.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/ModelFactory/Sky/CubeMap.h"
#include "Graphics/Grid/Grid.h"
#include "Graphics/Grid/Box.h"
#include "BasicPrimitives/Frustum.h"

// systems
#include "ECS/SystemManager/Systems/System/GridSystem/GridSystem.h"
#include "ECS/SystemManager/Systems/System/LightingSystem/LightingSystem.h"
#include "ECS/SystemManager/Systems/System/FileWatchSystem/FileWatchSystem.h"
#include "ECS/SystemManager/Systems/System/CameraSystem/CameraSystem.h"

namespace Eclipse
{
    class GraphicsManager
    {
    public:
        OpenGL_Context mRenderContext;
        std::vector<AssimpModel*> ModelContainer;
        InputWrapper InputHandler;
        GLenum GlobalMode = GL_FILL;
        std::unique_ptr<CubeMap> Sky;
        AABB_ AllAABBs;
        std::unique_ptr<Quad> GridQuad;

        unsigned int createdID;
        unsigned int SkyCount = 0;
        bool CheckRender = true;
        bool EnableGammaCorrection = true;
        bool DrawSky = false;
        float GammaCorrection = 2.2f;
        ECVec3 BackGroundColour{ 0.1f,0.2f,0.3f };
        float Exposure = 1.0f;
        bool VisualizeNormalVectors = false;
        float Magnitude = 0.2f;
        bool EnableNormalMapping = false;
        bool EnableEnvironmentMapForAll = false;

        // Seperate FBO for Post Process
        std::unique_ptr<FrameBuffer> PostProcess;
        // Seperate FBO for Post Process
        std::unique_ptr<FrameBuffer> Pong;

    public:
        std::map<std::string, std::vector<std::string>> ShaderMap;
        std::unordered_map<std::string, std::unique_ptr<std::thread>> GraphicThreads;

        void MassInit();
        void RegisterThreads();
        void Pre_Render();
        void Post_Render();
        void End();
        void Unload();
        void Draw(FrameBufferMode Mode, MeshComponent* _spritecomponent, GLenum mode, unsigned int ID, CameraComponent::CameraType _camType);
        void DrawIndexed(MeshComponent* in, GLenum mode);
        void CheckTexture(unsigned int ID);
        void CreatePrimitives(Entity ID, int ModelType);
        void CreateSky(std::string _Dir);
        static void LoadSky();
        void RenderSky(FrameBufferMode Mode);
        float GetGammaCorrection();
        void SetGammaCorrection(float in);
        void UploadGlobalUniforms();
        void CheckUniformLoc(Shader* _shdrpgm, MeshComponent& sprite, unsigned int id, CameraComponent& camera);
        void ResetInstancedDebugBoxes();
        void DrawDebugBoxes();
        std::string GetModelName(unsigned int modelname);
        static void WindowCloseCallback(GLFWwindow* window);
        void SetBackGroundColour();
        void FinalRender();
        void DrawEntireGrid();
        void PostProcessUpdate();

    private:
        static void CreateCompilerFolders();
        static void CreateEmptyFolder(std::string folderName, std::string folderPath = "..//Compiler//CompilerKeyFiles\\");
    };
}

#endif