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
  "   frag_color = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0";

const char *fragment_shader_source2 = "#version 330 core\n"
  "out vec4 frag_color;\n"
  "void main()\n"
  "{\n"
  "   frag_color = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
  "}\n\0";

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

  framebuffer_size_callback(window, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Graphic pipeline

  /*
  float vertex_data[] = {
    0.5f,  0.5f, 0.0f,  // 우측 상단
    0.5f, -0.5f, 0.0f,  // 우측 하단
    -0.5f, -0.5f, 0.0f,  // 좌측 하단
    -0.5f,  0.5f, 0.0f   // 좌측 상단
  };
  */

  float firstTriangle[] = {
    -0.9f, -0.5f, 0.0f,  // left 
    -0.0f, -0.5f, 0.0f,  // right
    -0.45f, 0.5f, 0.0f,  // top 
  };
  float secondTriangle[] = {
    0.0f, -0.5f, 0.0f,  // left
    0.9f, -0.5f, 0.0f,  // right
    0.45f, 0.5f, 0.0f   // top 
  };

  unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
    0, 1, 3,   // 첫 번째 삼각형
    1, 2, 3    // 두 번째 삼각형
  };  

  //////////////////////////////////////////////////////////////////////////////
  //                    Vetex Object, Vertex Array Object                    //
  //////////////////////////////////////////////////////////////////////////////

  // check variable
  int is_sucess;
  char info_log[512];
  
  unsigned int VAO[2], VBO[2], EBO;
  glGenVertexArrays(2, VAO);
  glGenBuffers(2, VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

  glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  //////////////////////////////////////////////////////////////////////////////
  //                             Vertex Shader                                //
  //////////////////////////////////////////////////////////////////////////////

  // create vertex shader
  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  // compile vertex shader
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

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

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_sucess);
  
  if (!is_sucess)
  {
    glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
    SPDLOG_ERROR("failed create fragment shader: {}", info_log);
  }

  // create fragment shader
  unsigned int fragment_shader2;
  fragment_shader2 = glCreateShader(GL_FRAGMENT_SHADER);

  // compile fragment shader
  glShaderSource(fragment_shader2, 1, &fragment_shader_source2, NULL);
  glCompileShader(fragment_shader2);

  glGetShaderiv(fragment_shader2, GL_COMPILE_STATUS, &is_sucess);
  
  if (!is_sucess)
  {
    glGetShaderInfoLog(fragment_shader2, 512, NULL, info_log);
    SPDLOG_ERROR("failed create fragment shader: {}", info_log);
  }

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

  // create shader program
  unsigned int shader_program2;
  shader_program2 = glCreateProgram();

  // attach shader with shader program
  glAttachShader(shader_program2, vertex_shader);
  glAttachShader(shader_program2, fragment_shader2);
  glLinkProgram(shader_program2);

  // check link shader
  glGetProgramiv(shader_program2, GL_LINK_STATUS, &is_sucess);
  if (!is_sucess)
  {
    glGetShaderInfoLog(shader_program2, 512, NULL, info_log);
    SPDLOG_ERROR("failed create vertex shader: {}", info_log);
  }


  //////////////////////////////////////////////////////////////////////////////
  //                                  Render                                  //
  //////////////////////////////////////////////////////////////////////////////
  

  SPDLOG_INFO("Start main loop");

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while(!glfwWindowShouldClose(window))
  {
    // Input
    on_key_event(window);

    // Render
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(VAO[0]);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shader_program2);

    glBindVertexArray(VAO[1]);
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