#include "context.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

void OnFramebufferSizeChange(GLFWwindow *window, int width, int height)
{
  SPDLOG_INFO("framebuffer size changed : ({} x {})", width, height);
  auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
  context->Reshape(width, height);
}

void OnKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
  SPDLOG_INFO("key : {}, scancode : {}, action : {}, mode : {}{}{}",
              key, scancode,
              action == GLFW_PRESS ? "Pressed" : action == GLFW_RELEASE ? "Released"
                                             : action == GLFW_REPEAT    ? "Repeat"
                                                                        : "Unknown",
              mods & GLFW_MOD_CONTROL ? "C" : "-",
              mods & GLFW_MOD_SHIFT ? "S" : "-",
              mods & GLFW_MOD_ALT ? "A" : "-");
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

void OnCharEvent (GLFWwindow* window, unsigned int ch)
{
  ImGui_ImplGlfw_CharCallback(window, ch);
}

void OnCursorPos(GLFWwindow* window, double x, double y)
{
  auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
  context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int modifier)
{
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);
  auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  context->MouseButton(button, action, x, y);
}

void OnScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

int main(int argc, const char **argv)
{
  // 한국어로 input받기위해서 필요
  SetConsoleCP(65001);
  // 한국어 출력하기 위해서 필요
  SetConsoleOutputCP(65001);
  SPDLOG_INFO("strat program");

  // glfw 라이브러리 초기화, 실패하면 에러 출력후 종료
  SPDLOG_INFO("Initialize glfw");
  if (!glfwInit())
  {
    const char *description = nullptr;
    glfwGetError(&description);
    SPDLOG_ERROR("failed to initialize glfw: {}", description);
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw 윈도우 생성, 실패하면 에러 출력후 종료
  SPDLOG_INFO("Create glfw window");
  auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
  if (!window)
  {
    SPDLOG_ERROR("failed to create glfw window");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // glad를 활용한 OpenGL함수 로딩
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    SPDLOG_ERROR("failed to initialize glad");
    glfwTerminate();
    return -1;
  }
  auto glVersion = glGetString(GL_VERSION);

	auto imguiContext = ImGui::CreateContext();
  ImGui::SetCurrentContext(imguiContext);
  ImGui_ImplGlfw_InitForOpenGL(window, false);
  ImGui_ImplOpenGL3_Init();
  ImGui_ImplOpenGL3_CreateFontsTexture();
  ImGui_ImplOpenGL3_CreateDeviceObjects();

  auto context = Context::Create();
  if (!context)
  {
    SPDLOG_ERROR("failed to create context");
    glfwTerminate();
    return -1;
  }
  glfwSetWindowUserPointer(window, context.get());

  OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
  glfwSetKeyCallback(window, OnKeyEvent);
  glfwSetCharCallback(window, OnCharEvent);
  glfwSetCursorPosCallback(window, OnCursorPos);
  glfwSetMouseButtonCallback(window, OnMouseButton);
  glfwSetScrollCallback(window, OnScroll);

  // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
  SPDLOG_INFO("Start main loop");
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    context->ProcessInput(window);
    context->Render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }
  // context메모리 해제
  context.reset();
  // context = nullptr;

  ImGui_ImplOpenGL3_DestroyFontsTexture();
  ImGui_ImplOpenGL3_DestroyDeviceObjects();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext(imguiContext);

  glfwTerminate();
  return 0;
}