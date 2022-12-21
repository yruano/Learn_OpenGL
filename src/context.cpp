#include "context.h"
#include "imgui.h"
#include <imgui.h>

ContextUPtr Context::Create() 
{
  auto context = ContextUPtr(new Context());
  if (!context->Init()) { return nullptr; }
  return std::move(context);
}

void Context::ProcessInput(GLFWwindow* window)
{
  if (!m_cameraControl) { return; }
  const float cameraSpeed = 0.005f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { m_cameraPos += cameraSpeed * m_cameraFront; }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { m_cameraPos -= cameraSpeed * m_cameraFront; }

  auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
  
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { m_cameraPos += cameraSpeed * cameraRight; }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { m_cameraPos -= cameraSpeed * cameraRight; }   

  auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { m_cameraPos += cameraSpeed * cameraUp; }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { m_cameraPos -= cameraSpeed * cameraUp; }
}

void Context::Reshape(int width, int height)
{
  m_width = width;
  m_height = height;
  glViewport(0, 0, m_width, m_height);
}

void Context::MouseMove(double x, double y) 
{
  if (!m_cameraControl) { return; }
  auto pos = glm::vec2((float)x, (float)y);
  auto deltaPos = pos - m_prevMousePos;

  const float cameraRotSpeed = 0.5f;
  m_cameraYaw -= deltaPos.x * cameraRotSpeed;
  m_cameraPitch -= deltaPos.y * cameraRotSpeed;

  if (m_cameraYaw < 0.0f) { m_cameraYaw += 360.0f; }
  if (m_cameraYaw > 360.0f) { m_cameraYaw -= 360.0f; }

  if (m_cameraPitch > 89.0f) { m_cameraPitch = 89.0f; }
  if (m_cameraPitch < -89.0f) { m_cameraPitch = -89.0f; }

  m_prevMousePos = pos;    
}

void Context::MouseButton(int button, int action, double x, double y)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT)
  {
    if (action == GLFW_PRESS)
    {
      m_prevMousePos = glm::vec2((float)x, (float) y);
      m_cameraControl = true;
    }
    else if (action == GLFW_RELEASE)
    {
      m_cameraControl = false;
    }
  }
}

bool Context::Init() 
{

  float vertices[] = 
  {
    -0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, // 0
     0.3f,  0.3f, -0.3f, 0.0f, 1.0f, 0.0f, // 1
     0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 1.0f, // 2
    -0.3f, -0.3f, -0.3f, 1.0f, 1.0f, 0.0f, // 3

    -0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, // 4
     0.3f,  0.3f,  0.3f, 0.0f, 1.0f, 0.0f, // 5
     0.3f, -0.3f,  0.3f, 0.0f, 0.0f, 1.0f, // 6
    -0.3f, -0.3f,  0.3f, 1.0f, 1.0f, 0.0f, // 7

    -0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, // 8
     0.3f, -0.3f,  0.3f, 0.0f, 1.0f, 0.0f, // 9
     0.3f, -0.3f, -0.3f, 0.0f, 0.0f, 1.0f, // 10
    -0.3f, -0.3f, -0.3f, 1.0f, 1.0f, 0.0f, // 11

     0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, // 12
     0.3f,  0.3f,  0.3f, 0.0f, 1.0f, 0.0f, // 13
     0.3f,  0.3f, -0.3f, 0.0f, 0.0f, 1.0f, // 14
     0.3f, -0.3f, -0.3f, 1.0f, 1.0f, 0.0f, // 15

    -0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, // 16
     0.3f,  0.3f,  0.3f, 0.0f, 1.0f, 0.0f, // 17
     0.3f,  0.3f, -0.3f, 0.0f, 0.0f, 1.0f, // 18
    -0.3f,  0.3f, -0.3f, 1.0f, 1.0f, 0.0f, // 19

    -0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, // 20
    -0.3f,  0.3f,  0.3f, 0.0f, 1.0f, 0.0f, // 21
    -0.3f,  0.3f, -0.3f, 0.0f, 0.0f, 1.0f, // 22
    -0.3f, -0.3f, -0.3f, 1.0f, 1.0f, 0.0f, // 23
  };

  uint32_t indices[] = 
  {
     0,  1,  2,  0,  2,  3,
     4,  5,  6,  4,  6,  7,
     8,  9, 10,  8, 10, 11,
    12, 13, 14, 12, 14, 15,
    16, 17, 18, 16, 18, 19,
    20, 21, 22, 20, 22, 23,
  };

  glGenVertexArrays(1, &m_vertexArrayObject);
  glBindVertexArray(m_vertexArrayObject);

  glGenBuffers(1, &m_vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 144, vertices, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
  
  glEnableVertexAttribArray(1);
  uint64_t offset = sizeof(float) * 3;
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)offset);

  glGenBuffers(1, &m_indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 36, indices, GL_DYNAMIC_DRAW);

  ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
  ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);

  if (!vertShader || !fragShader) { return false; }

  SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
  SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

  m_program = Program::Create({fragShader, vertShader});
  if (!m_program) { return false; }
  SPDLOG_INFO("program id: {}", m_program->Get());

  m_program->Use();

  glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
  return true;
}


void Context::Render() 
{
  if (ImGui::Begin("ui window"))
  {
    if (ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor)))
    {
      glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
    }
    ImGui::Separator();
    ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
    ImGui::DragFloat("camera yaw", &m_cameraYaw, 0.5f);
    ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
    ImGui::Separator();
    if (ImGui::Button("reset camera"))
    {
      m_cameraYaw = 0.0f;
      m_cameraPitch = 0.0f;
      m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    }
  }
  ImGui::End();

  std::vector<glm::vec3> cubePositions = 
  {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f),
  };
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  m_cameraFront = 
    glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) 
    * glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), 
    glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

  auto view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

  auto projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.01f, 20.0f);
  for (size_t i = 0; i < cubePositions.size(); i++)
  {
    auto& pos = cubePositions[i];
    auto model = glm::translate(glm::mat4(1.0f), pos);
    model = glm::rotate(model, glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i), glm::vec3(1.0f, 0.5f, 0.0f));
    auto transform = projection * view * model;
    m_program->SetUniform("transform", transform);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }
}