#include "pch.h"
#include "Grid.h"

void Grid::init()
{
    ID = engine->world.CreateEntity();
    engine->world.AddComponent(ID, TransformComponent{});

    Single = new Quad;
    ShaderRef = &(Graphics::shaderpgms.find("Grid")->second);
    modelRef = Graphics::models.find("square")->second.get();

    std::cout << Single->GetDrawCount() << std::endl;
}

void Grid::DrawGrid()
{
    CameraComponent camera;

    glBindFramebuffer(GL_FRAMEBUFFER, engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::SCENEVIEW)->GetFrameBufferID());

    ShaderRef->Use();

    // Part 2: Bind the object's VAO handle using glBindVertexArray
    glBindVertexArray(Single->GetVaoID());

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
    TransformComponent& trans = engine->world.GetComponent<TransformComponent>(ID);

    //GLint uniform_var_loc1 = ShaderRef->GetLocation("uModelToNDC");
    GLint uniform_var_loc2 = ShaderRef->GetLocation("viewMtx");
    GLint uniform_var_loc3 = ShaderRef->GetLocation("projMtx");
    //GLint uniform_var_loc4 = ShaderRef->GetLocation("position");


    glm::mat4 mModelNDC;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, trans.position.ConvertToGlmVec3Type());
    model = glm::rotate(model, glm::radians(trans.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(trans.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(trans.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, trans.scale.ConvertToGlmVec3Type());
    mModelNDC = camera.projMtx * camera.viewMtx * model;

    if (uniform_var_loc2 >= 0)
    {
        glUniformMatrix4fv(uniform_var_loc2, 1, GL_FALSE, glm::value_ptr(camera.viewMtx));
    }

    if (uniform_var_loc3 >= 0)
    {
        glUniformMatrix4fv(uniform_var_loc3, 1, GL_FALSE, glm::value_ptr(camera.projMtx));
    }

    //if (uniform_var_loc4 >= 0)
    //{
    //    GLCall(glUniform3f(uniform_var_loc4, trans.position.getX(), trans.position.getY(), trans.position.getZ()));
    //}

    glDrawElements(Single->GetPrimitiveType(), Single->GetDrawCount(), GL_UNSIGNED_SHORT, NULL);

    // Part 5: Clean up
    glBindVertexArray(0);
    ShaderRef->UnUse();
}
