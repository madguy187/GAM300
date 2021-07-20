#include "pch.h"
#include "MaterialManager.h"

namespace Eclipse
{
    Material Material::Emerald =
    {
        ECVec3(0.0215, 0.1745, 0.0215),
        ECVec3(0.07568, 0.61424, 0.07568),
        ECVec3(0.633, 0.727811, 0.633),
        0.6
    };

    Material Material::Jade =
    {
        ECVec3(0.135, 0.2225, 0.1575),
        ECVec3(0.54, 0.89, 0.63),
        ECVec3(0.316228, 0.316228, 0.316228),
        0.1
    };

    Material Material::Obsidian =
    {
        ECVec3(0.05375, 0.05, 0.06625),
        ECVec3(0.18275, 0.17, 0.22525),
        ECVec3(0.332741, 0.328634, 0.346435),
        0.3
    };

    Material Material::Pearl =
    {
        ECVec3(0.25, 0.20725, 0.20725),
        ECVec3(1, 0.829, 0.829),
        ECVec3(0.296648, 0.296648, 0.296648),
        0.088
    };

    Material Material::Ruby =
    {
        ECVec3(0.1745, 0.01175, 0.01175),
        ECVec3(0.61424, 0.04136, 0.04136),
        ECVec3(0.727811, 0.626959, 0.626959),
        0.6
    };

    Material Material::Turquoise =
    {
        ECVec3(0.1, 0.18725, 0.1745),
        ECVec3(0.396, 0.74151, 0.69102),
        ECVec3(0.297254, 0.30829, 0.306678),
        0.1
    };

    Material Material::Brass =
    {
        ECVec3(0.329412, 0.223529, 0.027451),
        ECVec3(0.780392, 0.568627, 0.113725),
        ECVec3(0.992157, 0.941176, 0.807843),
        0.21794872
    };

    Material Material::Bronze =
    {
        ECVec3(0.2125, 0.1275, 0.054),
        ECVec3(0.714, 0.4284, 0.18144),
        ECVec3(0.393548, 0.271906, 0.166721),
        0.2
    };

    Material Material::Chrome =
    {
        ECVec3(0.25, 0.25, 0.25),
        ECVec3(0.4, 0.4, 0.4),
        ECVec3(0.774597, 0.774597, 0.774597),
        0.6
    };

    Material Material::Copper =
    {
        ECVec3(0.19125, 0.0735, 0.0225),
        ECVec3(0.7038, 0.27048, 0.0828),
        ECVec3(0.256777, 0.137622, 0.086014),
        0.1
    };

    Material Material::Gold =
    {
        ECVec3(0.24725, 0.1995, 0.0745),
        ECVec3(0.75164, 0.60648, 0.22648),
        ECVec3(0.628281, 0.555802, 0.366065),
        0.4
    };

    Material Material::Silver =
    {
        ECVec3(0.19225, 0.19225, 0.19225),
        ECVec3(0.50754, 0.50754, 0.50754),
        ECVec3(0.508273, 0.508273, 0.508273),
        0.4
    };

    Material Material::Black_plastic =
    {
        ECVec3(0.0, 0.0, 0.0),
        ECVec3(0.01, 0.01, 0.01),
        ECVec3(0.50, 0.50, 0.50),
        .25
    };

    Material Material::Cyan_plastic =
    {
        ECVec3(0.0, 0.1, 0.06),
        ECVec3(0.0, 0.50980392, 0.50980392),
        ECVec3(0.50196078, 0.50196078, 0.50196078),
        .25
    };

    Material Material::Green_plastic =
    {
        ECVec3(0.0, 0.0, 0.0),
        ECVec3(0.1, 0.35, 0.1),
        ECVec3(0.45, 0.55, 0.45),
        .25
    };

    Material Material::Red_plastic =
    {
        ECVec3(0.0, 0.0, 0.0),
        ECVec3(0.5, 0.0, 0.0),
        ECVec3(0.7, 0.6, 0.6),
        .25
    };

    Material Material::White_plastic =
    {
        ECVec3(0.0, 0.0, 0.0),
        ECVec3(0.55, 0.55, 0.55),
        ECVec3(0.70, 0.70, 0.70),
        .25
    };
    Material Material::Yellow_plastic =
    {
        ECVec3(0.0, 0.0, 0.0),
        ECVec3(0.5, 0.5, 0.0),
        ECVec3(0.60, 0.60, 0.50),
        .25
    };

    Material Material::Black_rubber =
    {
        ECVec3(0.02, 0.02, 0.02),
        ECVec3(0.01, 0.01, 0.01),
        ECVec3(0.4, 0.4, 0.4),
        .078125
    };

    Material Material::Cyan_rubber =
    {
        ECVec3(0.0, 0.05, 0.05),
        ECVec3(0.4, 0.5, 0.5),
        ECVec3(0.04, 0.7, 0.7),
        .078125
    };

    Material Material::Green_rubber =
    {
        ECVec3(0.0, 0.05, 0.0),
        ECVec3(0.4, 0.5, 0.4),
        ECVec3(0.04, 0.7, 0.04),
        .078125
    };

    Material Material::Red_rubber =
    {
        ECVec3(0.05, 0.0, 0.0),
        ECVec3(0.5, 0.4, 0.4),
        ECVec3(0.7, 0.04, 0.04),
        .078125
    };
    Material Material::White_rubber =
    {
        ECVec3(0.05, 0.05, 0.05),
        ECVec3(0.5, 0.5, 0.5),
        ECVec3(0.7, 0.7, 0.7),
        .078125
    };

    Material Material::Yellow_rubber =
    {
        ECVec3(0.05, 0.05, 0.0),
        ECVec3(0.5, 0.5, 0.4),
        ECVec3(0.7, 0.7, 0.04),
        .078125
    };

    Material Material::mix(Material m1, Material m2, float mix)
    {
        return
        {
            m1.Ambient * mix + m2.Ambient * (1 - mix),
            m1.Diffuse * mix + m2.Diffuse * (1 - mix),
            m1.Specular * mix + m2.Specular * (1 - mix),
            m1.Shininess * mix + m2.Shininess * (1 - mix)
        };
    }

    float MaterialManager::GetCurrentShininess(MaterialComponent& in)
    {
        return (in.shininess * in.MaximumShininess);
    }

    MaterialManager::MaterialManager(bool InitRegisterHighlight_, bool EnableHighlight_) :
        InitRegisterHighlight(InitRegisterHighlight_),
        EnableHighlight(EnableHighlight_)
    {
    }


    void MaterialManager::CheckUnniformLocation(Shader& in, MaterialComponent& inside)
    {
        GLint uniform_var_loc1 = in.GetLocation("material.shininess");
        GLint uniform_var_loc2 = in.GetLocation("material.MaximumShininess");
        GLuint uniformloc3 = in.GetLocation("outlining");
        GLuint uniformloc4 = in.GetLocation("HighlightColour");

        GLCall(glUniform1f(uniform_var_loc1, inside.shininess));
        GLCall(glUniform1f(uniform_var_loc2, inside.MaximumShininess));
        GLCall(glUniform1f(uniformloc3, inside.Thickness));
        GLCall(glUniform3f(uniformloc4, inside.HighlightColour.getX(), inside.HighlightColour.getY(), inside.HighlightColour.getZ()));

    }

    void MaterialManager::CheckUniformLoc(RenderComponent& sprite, Shader& in, unsigned int id, unsigned int framebufferID)
    {
        CameraComponent camera;
        TransformComponent camerapos;

        if (framebufferID == engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID())
        {
            camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
            camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetEditorCameraID());
        }
        else
        {
            //Check if game camera exists
            if (engine->gCamera.GetGameCameraID() == MAX_ENTITY)
            {
                return;
            }

            camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
            camerapos = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetEditorCameraID());
        }

        TransformComponent& trans = engine->world.GetComponent<TransformComponent>(id);
        MaterialComponent& outline = engine->world.GetComponent<MaterialComponent>(id);

        GLint uniform_var_loc1 = in.GetLocation("uModelToNDC");

        if (uniform_var_loc1 >= 0)
        {
            glm::mat4 mModelNDC;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, trans.position.ConvertToGlmVec3Type());
            model = glm::rotate(model, glm::radians(trans.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(trans.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(trans.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, { trans.scale.getX() * outline.ScaleUp , trans.scale.getX() * outline.ScaleUp, trans.scale.getZ() * outline.ScaleUp });
            mModelNDC = camera.projMtx * camera.viewMtx * model;
            glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        }

    }

    void MaterialManager::UpdateShininess(MaterialComponent& in)
    {
        auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");
        shdrpgm->second.Use();
        CheckUnniformLocation(shdrpgm->second, in);
        shdrpgm->second.UnUse();
    }

    void MaterialManager::Highlight(unsigned int FrameBufferID, GLenum Mode)
    {
        if (EnableHighlight == true)
        {
            auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

            glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
            auto shdrpgm = Graphics::shaderpgms.find("OutLineShader");

            shdrpgm->second.Use();

            for (auto const& Models : ModelHighlightContainer)
            {
                auto& ID = Models.first;
                auto& InvidualModels = *(Models.second);

                auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
                auto& highlight = engine->world.GetComponent<MaterialComponent>(ID);

                if (highlight.Highlight == true)
                {
                    // Check Main Uniforms For each Model
                    // Translation done here for each model
                    engine->AssimpManager.CheckUniformLoc(shdrpgm->second, _camera, FrameBufferID, ID);

                    // Materials Update
                    engine->MaterialManager.CheckUnniformLocation(shdrpgm->second, highlight);

                    // Render
                    InvidualModels.Render(shdrpgm->second, Mode, FrameBufferID);
                }
            }

            shdrpgm->second.UnUse();
        }
    }

    void MaterialManager::RegisterForHighlighting(MaterialComponent& in, unsigned int index)
    {
        if (in.RegisterForHighlight == true)
            return;

        bool CheckModel = engine->world.CheckComponent<RenderComponent>(index);

        if (CheckModel)
        {

        }
        else
        {
            AssimpModel* CurrentModel = engine->AssimpManager.GetModel(index);

            // Insert
            if (ModelHighlightContainer.insert(std::pair<unsigned int, AssimpModel*>(index, CurrentModel)).second == true)
            {
                in.RegisterForHighlight = true;

                std::string Success = ("Model [" + CurrentModel->GetName() + "] Registered For Highlighting ! ").c_str();
                ENGINE_CORE_INFO(Success);

                std::cout << "HighLight Container Size : " << ModelHighlightContainer.size() << std::endl;
                return;
            }
        }
    }

    void MaterialManager::Highlight(unsigned int FrameBufferID, unsigned int ModelID, GLenum mode)
    {
        if (EnableHighlight == true)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

            auto shdrpgm = Graphics::shaderpgms.find("OutLineShader");

            shdrpgm->second.Use();

            auto& _spritecomponent = engine->world.GetComponent<RenderComponent>(ModelID);
            auto& highlight = engine->world.GetComponent<MaterialComponent>(ModelID);

            // Part 2: Bind the object's VAO handle using glBindVertexArray
            glBindVertexArray(_spritecomponent.modelRef->second->GetVaoID());

            glEnable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, mode);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            engine->GraphicsManager.CheckTexture(&_spritecomponent);

            // Materials Update
            engine->MaterialManager.CheckUnniformLocation(shdrpgm->second, highlight);

            CheckUniformLoc(_spritecomponent, shdrpgm->second, _spritecomponent.ID, FrameBufferID);

            engine->GraphicsManager.DrawIndexed(&_spritecomponent, GL_UNSIGNED_SHORT);

            // Part 5: Clean up
            glBindVertexArray(0);
            shdrpgm->second.UnUse();
        }
    }
}