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
    void draw();

    // to be removed
    void ImguiRender();
    void UpdateFrameBuffer();
    OpenGL_Context mRenderContext;
  };
}