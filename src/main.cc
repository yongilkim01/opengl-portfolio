#include "context.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void on_key_event(GLFWwindow* window);

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

  auto context = Context::create();
  if(!context)
  {
    SPDLOG_ERROR("failed to create context");
    glfwTerminate();
    return -1;
  }

  framebuffer_size_callback(window, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


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
    // Input
    on_key_event(window);

    context->render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  context.reset();

  glfwTerminate();
  
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
  SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
  glViewport(0, 0, width, height);
}

void on_key_event(GLFWwindow* window) 
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}