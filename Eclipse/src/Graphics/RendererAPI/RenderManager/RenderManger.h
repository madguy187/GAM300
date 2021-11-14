#pragma once
#include "Editor/Windows/Scene/SceneView.h"
#include "Editor/Windows/GameView/GameView.h"

#include "Editor/Windows/SwitchViews/TopSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/BottomSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/LeftSwitchViewWindow.h"
#include "Editor/Windows/SwitchViews/RightSwitchViewWindow.h"

namespace Eclipse
{
    class RenderManager
    {
    public:
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 0.1f, far_plane = 150.0f;

        // Normal Render Passes
        void RenderScene(MeshComponent& Mesh, Entity ID);
        void RenderGame(MeshComponent& Mesh, Entity ID);
        void RenderOtherViews(MeshComponent& Mesh, Entity ID);

        // Shadow
        void UpdateLightMatrix();
        void RenderSceneFromLightPOV(MeshComponent& , Entity );
        void RenderGameFromLightPOV(MeshComponent& Mesh, Entity entityID);
    };
}