#include "pch.h"
#include "MaterialManager.h"

namespace Eclipse
{
    Material Material::Emerald =
    {
        ECVec3(0.0215f, 0.1745f, 0.0215f),
        ECVec3(0.07568f, 0.61424f, 0.07568f),
        ECVec3(0.633f, 0.727811f, 0.633f),
        0.6f
    };

    Material Material::Jade =
    {
        ECVec3(0.135f, 0.2225f, 0.1575f),
        ECVec3(0.54f, 0.89f, 0.63f),
        ECVec3(0.316228f, 0.316228f, 0.316228f),
        0.1f
    };

    Material Material::Obsidian =
    {
        ECVec3(0.05375f, 0.05f, 0.06625f),
        ECVec3(0.18275f, 0.17f, 0.22525f),
        ECVec3(0.332741f, 0.328634f, 0.346435f),
        0.3f
    };

    Material Material::Pearl =
    {
        ECVec3(0.25f, 0.20725f, 0.20725f),
        ECVec3(1.0f, 0.829f, 0.829f),
        ECVec3(0.296648f, 0.296648f, 0.296648f),
        0.088f
    };

    Material Material::Ruby =
    {
        ECVec3(0.1745f, 0.01175f, 0.01175f),
        ECVec3(0.61424f, 0.04136f, 0.04136f),
        ECVec3(0.727811f, 0.626959f, 0.626959f),
        0.6f
    };

    Material Material::Turquoise =
    {
        ECVec3(0.1f, 0.18725f, 0.1745f),
        ECVec3(0.396f, 0.74151f, 0.69102f),
        ECVec3(0.297254f, 0.30829f, 0.306678f),
        0.1f
    };

    Material Material::Brass =
    {
        ECVec3(0.329412f, 0.223529f, 0.027451f),
        ECVec3(0.780392f, 0.568627f, 0.113725f),
        ECVec3(0.992157f, 0.941176f, 0.807843f),
        0.21794872f
    };

    Material Material::Bronze =
    {
        ECVec3(0.2125f, 0.1275f, 0.054f),
        ECVec3(0.714f, 0.4284f, 0.18144f),
        ECVec3(0.393548f, 0.271906f, 0.166721f),
        0.2f
    };

    Material Material::Chrome =
    {
        ECVec3(0.25f, 0.25f, 0.25f),
        ECVec3(0.4f, 0.4f, 0.4f),
        ECVec3(0.774597f, 0.774597f, 0.774597f),
        0.6f
    };

    Material Material::Copper =
    {
        ECVec3(0.19125f, 0.0735f, 0.0225f),
        ECVec3(0.7038f, 0.27048f, 0.0828f),
        ECVec3(0.256777f, 0.137622f, 0.086014f),
        0.1f
    };

    Material Material::Gold =
    {
        ECVec3(0.24725f, 0.1995f, 0.0745f),
        ECVec3(0.75164f, 0.60648f, 0.22648f),
        ECVec3(0.628281f, 0.555802f, 0.366065f),
        0.4f
    };

    Material Material::Silver =
    {
        ECVec3(0.19225f, 0.19225f, 0.19225f),
        ECVec3(0.50754f, 0.50754f, 0.50754f),
        ECVec3(0.508273f, 0.508273f, 0.508273f),
        0.4f
    };

    Material Material::Black_plastic =
    {
        ECVec3(0.0f, 0.0f, 0.0f),
        ECVec3(0.01f, 0.01f, 0.01f),
        ECVec3(0.50f, 0.50f, 0.50f),
        0.25f
    };

    Material Material::Cyan_plastic =
    {
        ECVec3(0.0f, 0.1f, 0.06f),
        ECVec3(0.0f, 0.50980392f, 0.50980392f),
        ECVec3(0.50196078f, 0.50196078f, 0.50196078f),
        0.25f
    };

    Material Material::Green_plastic =
    {
        ECVec3(0.0f, 0.0f, 0.0f),
        ECVec3(0.1f, 0.35f, 0.1f),
        ECVec3(0.45f, 0.55f, 0.45f),
        0.25f
    };

    Material Material::Red_plastic =
    {
        ECVec3(0.0f, 0.0f, 0.0f),
        ECVec3(0.5f, 0.0f, 0.0f),
        ECVec3(0.7f, 0.6f, 0.6f),
        0.25f
    };

    Material Material::White_plastic =
    {
        ECVec3(0.0f, 0.0f, 0.0f),
        ECVec3(0.55f, 0.55f, 0.55f),
        ECVec3(0.70f, 0.70f, 0.70f),
        0.25f
    };
    Material Material::Yellow_plastic =
    {
        ECVec3(0.0f, 0.0f, 0.0f),
        ECVec3(0.5f, 0.5f, 0.0f),
        ECVec3(0.60f, 0.60f, 0.50f),
        0.25f
    };

    Material Material::Black_rubber =
    {
        ECVec3(0.02f, 0.02f, 0.02f),
        ECVec3(0.01f, 0.01f, 0.01f),
        ECVec3(0.4f, 0.4f, 0.4f),
        0.078125f
    };

    Material Material::Cyan_rubber =
    {
        ECVec3(0.0f, 0.05f, 0.05f),
        ECVec3(0.4f, 0.5f, 0.5f),
        ECVec3(0.04f, 0.7f, 0.7f),
        0.078125f
    };

    Material Material::Green_rubber =
    {
        ECVec3(0.0f, 0.05f, 0.0f),
        ECVec3(0.4f, 0.5f, 0.4f),
        ECVec3(0.04f, 0.7f, 0.04f),
        0.078125f
    };

    Material Material::Red_rubber =
    {
        ECVec3(0.05f, 0.0f, 0.0f),
        ECVec3(0.5f, 0.4f, 0.4f),
        ECVec3(0.7f, 0.04f, 0.04f),
        0.078125f
    };

    Material Material::White_rubber =
    {
        ECVec3(0.05f, 0.05f, 0.05f),
        ECVec3(0.5f, 0.5f, 0.5f),
        ECVec3(0.7f, 0.7f, 0.7f),
        0.078125f
    };

    Material Material::Yellow_rubber =
    {
        ECVec3(0.05f, 0.05f, 0.0f),
        ECVec3(0.5f, 0.5f, 0.4f),
        ECVec3(0.7f, 0.7f, 0.04f),
        .078125f
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
        GLCall(glUniform1f(uniformloc3, engine->MaterialManager.Thickness));

        GLCall(glUniform3f(uniformloc4,
            engine->MaterialManager.HighlightColour.getX(),
            engine->MaterialManager.HighlightColour.getY(),
            engine->MaterialManager.HighlightColour.getZ()));
    }

    void MaterialManager::CheckUniformLoc(MeshComponent& sprite, Shader& in, unsigned int id, FrameBufferMode Mode)
    {
        (void)sprite;

        CameraComponent camera;
        TransformComponent camerapos;

        if (Mode == FrameBufferMode::FBM_SCENE)
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

        GLint uniform_var_loc1 = in.GetLocation("uModelToNDC");

        if (uniform_var_loc1 >= 0)
        {
            glm::mat4 mModelNDC;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, trans.position.ConvertToGlmVec3Type());
            model = glm::rotate(model, glm::radians(trans.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(trans.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(trans.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, { trans.scale.getX() * engine->MaterialManager.ScaleUp , trans.scale.getY() * engine->MaterialManager.ScaleUp, trans.scale.getZ() * engine->MaterialManager.ScaleUp });
            mModelNDC = camera.projMtx * camera.viewMtx * model;
            glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        }

    }

    void MaterialManager::HighlightBasicPrimitives(unsigned int EntityId, FrameBufferMode Mode)
    {
        if (!engine->world.CheckComponent< MaterialComponent>(EntityId))
            return;

        auto& Mesh = engine->world.GetComponent<MaterialComponent>(EntityId);

        if (Mesh.Highlight == true)
        {
            if (engine->world.CheckComponent<MeshComponent>(EntityId))
            {
                engine->MaterialManager.UpdateStencilWith_Outline(EntityId);
                engine->MaterialManager.Highlight(Mode, EntityId, GL_FILL);
            }
        }
    }

    void MaterialManager::Highlight3DModels(unsigned int EntityId, FrameBufferMode Mode)
    {
        if (!engine->world.CheckComponent< MaterialComponent>(EntityId))
            return;

        auto& Mesh = engine->world.GetComponent<MaterialComponent>(EntityId);

        if (Mesh.Highlight == true)
        {
            engine->MaterialManager.UpdateStencilWith_Outline(EntityId);
            engine->MaterialManager.Highlight3DModels(Mode, EntityId, GL_FILL);
        }
    }

    void MaterialManager::UpdateShininess(unsigned int EntityID)
    {
        if (!engine->world.CheckComponent< MaterialComponent>(EntityID))
            return;

        auto& Mat = engine->world.GetComponent<MaterialComponent>(EntityID);

        auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");
        shdrpgm->second.Use();
        CheckUnniformLocation(shdrpgm->second, Mat);
        shdrpgm->second.UnUse();
    }

    void MaterialManager::Highlight(FrameBufferMode Mode, unsigned int ModelID, GLenum mode)
    {
        if (EnableHighlight == true)
        {
            engine->gFrameBufferManager->UseFrameBuffer(Mode);

            auto& shdrpgm = Graphics::shaderpgms["OutLineShader"];

            shdrpgm.Use();

            auto& _spritecomponent = engine->world.GetComponent<MeshComponent>(ModelID);
            auto& highlight = engine->world.GetComponent<MaterialComponent>(ModelID);

            // Part 2: Bind the object's VAO handle using glBindVertexArray          
            glBindVertexArray(Graphics::models[_spritecomponent.modelRef]->GetVaoID());

            glEnable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, mode);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            engine->GraphicsManager.CheckTexture(ModelID);

            // Materials Update
            engine->MaterialManager.CheckUnniformLocation(shdrpgm, highlight);

            CheckUniformLoc(_spritecomponent, shdrpgm, ModelID, Mode);

            engine->GraphicsManager.DrawIndexed(&_spritecomponent, GL_UNSIGNED_SHORT);

            // Part 5: Clean up
            glBindVertexArray(0);
            shdrpgm.UnUse();
        }
    }

    bool MaterialManager::HighlightClick(unsigned int ModelID)
    {
        if (engine->world.CheckComponent<MaterialComponent>(ModelID) == true)
        {
            MaterialComponent& PrepareToHighlight = engine->world.GetComponent<MaterialComponent>(ModelID);

            if (PrepareToHighlight.Modeltype == MaterialModelType::MT_BASIC ||
                PrepareToHighlight.Modeltype == MaterialModelType::MT_MODELS3D)
            {
                PrepareToHighlight.Highlight = true;
                return true;
            }

            return false;
        }

        return false;
    }

    bool MaterialManager::UnHighlight(unsigned int ModelID)
    {
        if (engine->world.CheckComponent<MaterialComponent>(ModelID) == true)
        {
            MaterialComponent& PrepareToHighlight = engine->world.GetComponent<MaterialComponent>(ModelID);

            if (PrepareToHighlight.Highlight == true)
            {
                PrepareToHighlight.Highlight = false;
                return true;
            }

            return false;
        }

        return false;
    }

    void MaterialManager::ClearContainer()
    {
        MeshHighLightContainer.clear();
    }

    void MaterialManager::UpdateStencilWithActualObject(unsigned int ID)
    {
        // We allow EACH object to pass the stencil test uniquely
        // Stencil updated uniquely with each mode's ID
        glStencilFunc(GL_ALWAYS, ID, 0xFF);

        // We enable writing to stencil buffer
        glStencilMask(0xFF);
    }

    void MaterialManager::DoNotUpdateStencil()
    {
        // We dont Update Stencil Buffer
        glStencilMask(0x00);
    }

    void MaterialManager::UpdateStencilWith_Outline(unsigned int ID)
    {
        // Draw parts of the Model that are not equal to the unique ID.
        // We only draw the part of the Model that are outside the previously drawn Model
        glStencilFunc(GL_NOTEQUAL, ID, 0xFF);
        glStencilMask(0xFF);
        glDisable(GL_DEPTH_TEST);
    }

    void MaterialManager::StencilBufferClear()
    {
        //Enable modifying of the stencil buffer
        glStencilMask(0xFF);

        // Clear stencil buffer
        glStencilFunc(GL_ALWAYS, 0, 0xFF);

        // Enable the depth buffer
        glEnable(GL_DEPTH_TEST);
    }

    void MaterialManager::CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int ModelID)
    {
        MaterialComponent& material = engine->world.GetComponent<MaterialComponent>(ModelID);
        GLint uniform_var_loc1 = _shdrpgm.GetLocation("material.shininess");
        GLint uniform_var_loc2 = _shdrpgm.GetLocation("material.MaximumShininess");

        GLCall(glUniform1f(uniform_var_loc1, material.shininess));
        GLCall(glUniform1f(uniform_var_loc2, material.MaximumShininess));


        TransformComponent& Transform = engine->world.GetComponent<TransformComponent>(ModelID);
        GLint uModelToNDC_ = _shdrpgm.GetLocation("uModelToNDC");
        GLuint model_ = _shdrpgm.GetLocation("model");
        GLuint dsa = _shdrpgm.GetLocation("noTex");

        if (uModelToNDC_ >= 0)
        {
            glm::mat4 mModelNDC;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, Transform.position.ConvertToGlmVec3Type());
            model = model * Transform.UpdateRotation();
            model = glm::scale(model, Transform.scale.ConvertToGlmVec3Type());
            Transform.ModelMatrix = model;

            mModelNDC = _camera.projMtx * _camera.viewMtx * model;
            glUniformMatrix4fv(uModelToNDC_, 1, GL_FALSE, glm::value_ptr(mModelNDC));
            glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        }

        glUniform1i(dsa, 0);
    }

    void MaterialManager::MeshHighlight(unsigned int FrameBufferID, GLenum Mode)
    {
        (void)Mode;

        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

        glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
        auto shdrpgm = Graphics::shaderpgms["OutLineShader"];

        shdrpgm.Use();

        for (auto const& Models : MeshHighLightContainer)
        {
            auto& ID = Models.first;

            engine->MaterialManager.UpdateStencilWith_Outline(ID);

            auto& highlight = engine->world.GetComponent<MaterialComponent>(ID);

            if (highlight.Highlight == true)
            {
                // Check Main Uniforms For each Model
                // Translation done here for each model
                CheckUniformLoc(shdrpgm, _camera, ID);

                // Materials Update
                engine->MaterialManager.CheckUnniformLocation(shdrpgm, highlight);

                // Render
                //engine->AssimpManager.Render(shdrpgm, Mode, FrameBufferID, InvidualModels, ID);
            }
        }

        shdrpgm.UnUse();
    }

    void MaterialManager::UpdateMaterial(MaterialComponent& in)
    {
        auto& shdrpgm = Graphics::shaderpgms["shader3DShdrpgm"];
        shdrpgm.Use();

        GLuint DifuseMaterial = shdrpgm.GetLocation("sdiffuse");
        GLuint SpecularMaterial = shdrpgm.GetLocation("sspecular");
        GLuint NoTextures = shdrpgm.GetLocation("noTex");
        GLint uniform_var_loc1 = shdrpgm.GetLocation("BasicPrimitives");


        glUniform1i(uniform_var_loc1, true);
        glUniform1i(NoTextures, in.NoTextures);
        glUniform4f(DifuseMaterial, in.diffuse.getX(), in.diffuse.getY(), in.diffuse.getZ(), 1);
        glUniform4f(SpecularMaterial, in.specular.getX(), in.specular.getY(), in.specular.getZ(), 1);

        shdrpgm.UnUse();
    }

    void MaterialManager::Highlight3DModels(FrameBufferMode Mode, unsigned int ModelID, GLenum mode)
    {
        if (EnableHighlight == true)
        {
            auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
            engine->gFrameBufferManager->UseFrameBuffer(Mode);

            auto shdrpgm = Graphics::shaderpgms["OutLineShader"];
            shdrpgm.Use();

            if (engine->world.CheckComponent<MaterialComponent>(ModelID))
            {
                auto& highlight = engine->world.GetComponent<MaterialComponent>(ModelID);

                // Check Main Uniforms For each Model
                // Translation done here for each model
                CheckUniformLoc(shdrpgm, _camera, ModelID);

                // Materials Update
                engine->MaterialManager.CheckUnniformLocation(shdrpgm, highlight);
            }

            // Render
            if (engine->world.CheckComponent<MeshComponent>(ModelID))
            {
                auto& InvidualModels = engine->world.GetComponent<MeshComponent>(ModelID);
                engine->AssimpManager.Render(mode, InvidualModels);
            }

            // Part 5: Clean up
            glBindVertexArray(0);
            shdrpgm.UnUse();
            engine->gFrameBufferManager->UnBind(Mode);
        }
    }
}