#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertex_shader_source = "#version 330 core\n"
  "layout (location = 0) in vec3 a_pos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
  "}\0";

const char *fragment_shader_source = "#version 330 core\n"
  "out vec4 frag_color;\n"
  "void main()\n"
  "{\n"
  "   frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0";


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

  framebuffer_size_callback(window, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Graphic pipeline

  float vertex_data[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
  };

  //////////////////////////////////////////////////////////////////////////////
  //                            Vertex Array Object                           //
  //////////////////////////////////////////////////////////////////////////////
  
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

  //////////////////////////////////////////////////////////////////////////////
  //                             Vertex Shader                                //
  //////////////////////////////////////////////////////////////////////////////

  // create vertex shader
  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  // compile vertex shader
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

  // check vertex shader creation
  int is_sucess;
  char info_log[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_sucess);
  if (!is_sucess)
  {
    glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
    SPDLOG_ERROR("failed create vertex shader: {}", info_log);
  }

  //////////////////////////////////////////////////////////////////////////////
  //                             Fragment Shader                              //
  //////////////////////////////////////////////////////////////////////////////

  // create fragment shader
  unsigned int fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  // compile fragment shader
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);

  //////////////////////////////////////////////////////////////////////////////
  //                             Shader program                               //
  //////////////////////////////////////////////////////////////////////////////

  // create shader program
  unsigned int shader_program;
  shader_program = glCreateProgram();

  // attach shader with shader program
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  // check link shader
  glGetProgramiv(shader_program, GL_LINK_STATUS, &is_sucess);
  if (!is_sucess)
  {
    glGetShaderInfoLog(shader_program, 512, NULL, info_log);
    SPDLOG_ERROR("failed create vertex shader: {}", info_log);
  }

  SPDLOG_INFO("Sucess create shader program!");

  //////////////////////////////////////////////////////////////////////////////
  //                          Linking Vetex Attribute                         //
  //////////////////////////////////////////////////////////////////////////////

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  SPDLOG_INFO("Sucess linking vertex pointer!");

  SPDLOG_INFO("Start main loop");
  while(!glfwWindowShouldClose(window))
  {
    // Input
    on_key_event(window);

    // Render
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // delete shader object
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  glfwTerminate();
  
  return 0;
}