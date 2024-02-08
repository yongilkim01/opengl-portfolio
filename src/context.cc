#include "context.h"

ContextUniquePtr Context::create()
{
    auto context = ContextUniquePtr(new Context());
    if(!context->init())
        return nullptr;
    return std::move(context);
}

bool Context::init()
{
    // create shader
    ShaderSharedPtr vertex_shader = Shader::create_from_file("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderSharedPtr fragment_shader = Shader::create_from_file("./shader/simple.fs", GL_FRAGMENT_SHADER);

    if(!vertex_shader || !fragment_shader) return false;

    SPDLOG_INFO("vertex shader id: {}", vertex_shader->get());
    SPDLOG_INFO("fragment shader id: {}", fragment_shader->get());

    // create shader program
    m_shader_program = ShaderProgram::create({fragment_shader, vertex_shader});

    if(!vertex_shader || !fragment_shader) return false;

    SPDLOG_INFO("shader program id: {}", m_shader_program->get());

    // clear background color
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    // test VAO
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

    return true;
}

void Context::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float time_value = glfwGetTime();
    float green_value = (sin(time_value) / 2.0f) + 0.5f;
    int vertex_color_loc = glGetUniformLocation(m_shader_program->get(), "our_color");

    glUseProgram(m_shader_program->get());
    glDrawArrays(GL_TRIANGLES, 0, 3);
}