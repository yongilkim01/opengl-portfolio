#include "context.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void on_key_event(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_pos(GLFWwindow* window, double x, double y);
void mouse_button(GLFWwindow* window, int button, int action, int modifier);
void on_char_event(GLFWwindow* window, unsigned int ch);
void on_scroll(GLFWwindow* window, double xoffset, double yoffset);

int main (int argc, const char** argv)
{
  SPDLOG_INFO("glfw init");

  // glfw 윈도우 생성
  if (!glfwInit())
  {
    const char* error_msg = nullptr;
    glfwGetError(&error_msg);
    SPDLOG_ERROR("fail glfw initialize: {}", error_msg);
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  SPDLOG_INFO("glfw window create");
  auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
  if (!window)
  {
    SPDLOG_ERROR("failed glfw window create");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  
  // glad를 활용한 OpenGL 함수 로딩
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
  {
    SPDLOG_ERROR("failed to initialize glad");
    glfwTerminate();
    return -1;
  }

  auto glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  SPDLOG_INFO("OpenGL context version: {}", glVersion);

  auto imgui_context = ImGui::CreateContext();
  ImGui::SetCurrentContext(imgui_context);
  ImGui_ImplGlfw_InitForOpenGL(window, false);
  ImGui_ImplOpenGL3_Init();
  ImGui_ImplOpenGL3_CreateFontsTexture();
  ImGui_ImplOpenGL3_CreateDeviceObjects();

  auto context = Context::create();
  if(!context)
  {
    SPDLOG_ERROR("failed to create context");
    glfwTerminate();
    return -1;
  }

  // set user pointer
  glfwSetWindowUserPointer(window, context.get());

  framebuffer_size_callback(window, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, cursor_pos);
  glfwSetMouseButtonCallback(window, mouse_button);
  glfwSetKeyCallback(window, on_key_event);
  glfwSetCharCallback(window, on_char_event);
  glfwSetScrollCallback(window, on_scroll);

  //////////////////////////////////////////////////////////////////////////////
  //                    Vetex Object, Vertex Array Object                    //
  //////////////////////////////////////////////////////////////////////////////
  /*
  float vertex_data[] = {
    // 위치              // 컬러
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 우측 하단
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 좌측 하단
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 위 
  };

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

  // 위치 attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // 컬러 attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  */

  //////////////////////////////////////////////////////////////////////////////
  //                                  Render                                  //
  //////////////////////////////////////////////////////////////////////////////
  

  SPDLOG_INFO("Start main loop");

  // 선 그리기로 변경
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    context->input(window);
    context->render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  context.reset();

  ImGui_ImplOpenGL3_DestroyFontsTexture();
  ImGui_ImplOpenGL3_DestroyDeviceObjects();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext(imgui_context);

  glfwTerminate();
  
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
  SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
  auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
  context->reshape(width, height);
}

void on_key_event(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void cursor_pos(GLFWwindow* window, double x, double y)
{
  auto context = (Context*)glfwGetWindowUserPointer(window);
  context->mouse_move(x, y);
}

void mouse_button(GLFWwindow* window, int button, int action, int modifier)
{
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);
  auto context = (Context*)glfwGetWindowUserPointer(window);
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  context->mouse_click(button, action, x, y);
}

void on_char_event(GLFWwindow* window, unsigned int ch) 
{
  ImGui_ImplGlfw_CharCallback(window, ch);
}

void on_scroll(GLFWwindow* window, double xoffset, double yoffset) 
{
  ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}