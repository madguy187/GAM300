#pragma once

namespace Eclipse
{
  class GraphicsManager
  {
  public:
    void pre_render();
    void post_render();
    void end();
    void unload();
    void FrameBufferDraw();

    // to be removed
    void ImguiRender();
    void UpdateFrameBuffer();
    OpenGL_Context mRenderContext;
    unsigned int createdID;
    void DrawBuffers(unsigned int FrameBufferID, Sprite* _spritecomponent, GLenum mode);

  private:
    void ShowTestWidgets();
    void CreatePrimitives(GLint model);
    void CheckUniformLoc(Sprite& sprite, unsigned int id, unsigned int framebufferID);

  public:
    void ShowTestWidgets(unsigned int id, unsigned int createdId);
  };
}