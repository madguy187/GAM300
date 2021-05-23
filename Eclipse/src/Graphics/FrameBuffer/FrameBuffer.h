#ifndef VIEW_H
#define VIEW_H
#include "Vec.h"

class World;

struct windowSpecifications
{
  float winWidth = 0.0f;
  float winHeight = 0.0f;

};

class FrameBuffer
{
public:
  FrameBuffer(const glm::uvec2& p_size);
  FrameBuffer(unsigned int p_width, unsigned int p_height);
  ~FrameBuffer();

  void Bind() const;
  void Unbind() const;
  void Clear() const;

  void Init();
  void Update();
  static void ShowWindow(FrameBuffer g);
  void CreateFrameBuffer(unsigned int p_width, unsigned int p_height);

  unsigned int GetGameViewBuffer();
  unsigned int GetTextureColourBuffer();

private:
  struct FramebufferData
  {
    unsigned int frameBufferID;
    unsigned int depthBufferID;
    unsigned int TextureColourBuffer;
  };

public:
  FramebufferData m_data;
  vec2 windowPos;
  glm::uvec2 m_size;
  unsigned int m_width, m_height;
  vec2 frameBufferPosition;
};

#endif//VIEW_H