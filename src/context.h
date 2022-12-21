#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"

CLASS_PTR(Context)
class Context
{
public:
  static ContextUPtr Create();
  void Render();
  void ProcessInput(GLFWwindow* window);
  void Reshape(int width, int height);
  void MouseMove(double x, double y);
  void MouseButton(int button, int action, double x, double y);

private:
  Context() {}
  bool Init();
  ProgramUPtr m_program;

  uint32_t m_vertexArrayObject;
  uint32_t m_vertexBuffer;
  uint32_t m_indexBuffer;

  // clear color
  glm::vec4 m_clearColor { glm:: vec4(0.1f, 0.2f, 0.3f, 0.0f) };

  // camera parameter
  bool m_cameraControl { false };
  glm::vec2 m_prevMousePos { glm::vec2(0.0f) };
  float m_cameraPitch { 0.0f };
  float m_cameraYaw { 0.0f };
  glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
  glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
  glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };

  int m_width { WINDOW_WIDTH };
  int m_height { WINDOW_HEIGHT };
};

#endif // __CONTEXT_H__