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

    void MaterialManager::CheckUniformLoc(MeshComponent& sprite, Shader& in, unsigned int id, unsigned int framebufferID)
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
            model = glm::scale(model, { trans.scale.getX() * outline.ScaleUp , trans.scale.getY() * outline.ScaleUp, trans.scale.getZ() * outline.ScaleUp });
            mModelNDC = camera.projMtx * camera.viewMtx * model;
            glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mModelNDC));
        }

    }

    void MaterialManager::HighlightBasicPrimitives(MaterialComponent& in, unsigned int EntityId, unsigned int FrameBufferID)
    {
        if (in.Highlight == true)
        {
            if (engine->world.CheckComponent<MeshComponent>(EntityId))
            {
                engine->MaterialManager.UpdateStencilWith_Outline(EntityId);
                engine->MaterialManager.Highlight(FrameBufferID, EntityId, GL_FILL);
            }
        }
    }

    void MaterialManager::Highlight3DModels(MaterialComponent& in, unsigned int EntityId, unsigned int FrameBufferID)
    {
        if (in.Highlight == true)
        {
            engine->MaterialManager.UpdateStencilWith_Outline(EntityId);
            engine->MaterialManager.Highlight3DModels(FrameBufferID, EntityId, GL_FILL);
        }
    }

    void MaterialManager::UpdateShininess(MaterialComponent& in)
    {
        auto shdrpgm = Graphics::shaderpgms.find("shader3DShdrpgm");
        shdrpgm->second.Use();
        CheckUnniformLocation(shdrpgm->second, in);
        shdrpgm->second.UnUse();
    }

    void MaterialManager::Highlight(unsigned int FrameBufferID, unsigned int ModelID, GLenum mode)
    {
        if (EnableHighlight == true)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

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

            CheckUniformLoc(_spritecomponent, shdrpgm, _spritecomponent.ID, FrameBufferID);

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

            if (PrepareToHighlight.Modeltype == MaterialComponent::ModelType::BasicPrimitives ||
                PrepareToHighlight.Modeltype == MaterialComponent::ModelType::Models3D)
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

    void MaterialManager::RegisterMeshForHighlight(unsigned int ID)
    {
        if (InitRegisterHighlight == true)
        {
            RegisterMeshForHighlighting(ID);
        }
    }

    void MaterialManager::ClearContainer()
    {
        MeshHighLightContainer.clear();
    }

    bool MaterialManager::InsertContainer(MeshComponent& in)
    {
        //// Insert
        //if (MeshHighLightContainer.insert({ in.ID , &in }).second == true)
        //{
        //    return true;
        //}

        return false;
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

    void MaterialManager::CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int FrameBufferID, unsigned int ModelID)
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
            model = glm::rotate(model, glm::radians(Transform.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(Transform.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(Transform.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, Transform.scale.ConvertToGlmVec3Type());

            mModelNDC = _camera.projMtx * _camera.viewMtx * model;
            glUniformMatrix4fv(uModelToNDC_, 1, GL_FALSE, glm::value_ptr(mModelNDC));
            glUniformMatrix4fv(model_, 1, GL_FALSE, glm::value_ptr(model));
        }

        glUniform1i(dsa, 0);
    }

    void MaterialManager::RegisterMeshForHighlighting(unsigned int index)
    {
        auto& mat = engine->world.GetComponent<MaterialComponent>(index);

        if (mat.RegisterForHighlight == true)
            return;

        auto& mesh = engine->world.GetComponent<MeshComponent>(index);
        auto& ModelInfo = engine->world.GetComponent<ModeLInforComponent>(index);

        // Insert
        if (MeshHighLightContainer.insert(std::pair<unsigned int, MeshComponent*>(index, &mesh)).second == true)
        {
            mat.RegisterForHighlight = true;

            std::string Success = ("Model [" + ModelInfo.NameOfModel + "] Registered For Highlighting ! ").c_str();
            ENGINE_CORE_INFO(Success);

            std::cout << "HighLight Container Size : " << ModelHighlightContainer.size() << std::endl;
            return;
        }
    }

    void MaterialManager::MeshHighlight(unsigned int FrameBufferID, GLenum Mode)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

        glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
        auto shdrpgm = Graphics::shaderpgms["OutLineShader"];

        shdrpgm.Use();

        for (auto const& Models : MeshHighLightContainer)
        {
            auto& ID = Models.first;
            auto& InvidualModels = *(Models.second);

            engine->MaterialManager.UpdateStencilWith_Outline(ID);

            auto& highlight = engine->world.GetComponent<MaterialComponent>(ID);

            if (highlight.Highlight == true)
            {
                auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

                // Check Main Uniforms For each Model
                // Translation done here for each model
                CheckUniformLoc(shdrpgm, _camera, FrameBufferID, ID);

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

        glUniform1i(NoTextures, in.NoTextures);
        glUniform4f(DifuseMaterial, in.diffuse.getX(), in.diffuse.getY(), in.diffuse.getZ(), 1);
        glUniform4f(SpecularMaterial, in.specular.getX(), in.specular.getY(), in.specular.getZ(), 1);

        shdrpgm.UnUse();
    }

    void MaterialManager::Highlight3DModels(unsigned int FrameBufferID, unsigned int ModelID, GLenum mode)
    {
        if (EnableHighlight == true)
        {
            auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

            glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
            auto shdrpgm = Graphics::shaderpgms["OutLineShader"];
            shdrpgm.Use();

            if (engine->world.CheckComponent<MaterialComponent>(ModelID))
            {
                auto& highlight = engine->world.GetComponent<MaterialComponent>(ModelID);

                // Check Main Uniforms For each Model
                // Translation done here for each model
                CheckUniformLoc(shdrpgm, _camera, FrameBufferID, ModelID);

                // Materials Update
                engine->MaterialManager.CheckUnniformLocation(shdrpgm, highlight);
            }

            // Render
            if (engine->world.CheckComponent<MeshComponent>(ModelID))
            {
                auto& InvidualModels = engine->world.GetComponent<MeshComponent>(ModelID);
                engine->AssimpManager.Render(shdrpgm, mode, FrameBufferID, InvidualModels, ModelID);
            }

            // Part 5: Clean up
            glBindVertexArray(0);
            shdrpgm.UnUse();
        }
    }
}