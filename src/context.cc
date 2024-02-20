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
    float vertex_data[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f
    };

    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // create vertex object
    m_vertex_layout = VertexLayout::create();

    // create vertex array object
    m_vertex_buffer = Buffer::create_with_data(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertex_data, sizeof(float) * 24);

    // set position attribute
    m_vertex_layout->set_attrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    m_vertex_layout->set_attrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);
    // color attribute
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // create index buffer
    m_index_buffer = Buffer::create_with_data(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

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

    return true;
}

void Context::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float time_value = glfwGetTime();
    float green_value = (sin(time_value) / 2.0f) + 0.5f;
    //int vertex_color_loc = glGetUniformLocation(m_shader_program->get(), "a_color");
    //glUniform3f(vertex_color_loc, green_value * green_value, 2.0f * green_value * (1.0f - time_value), (1.0f - time_value) * (1.0f - time_value));

    m_shader_program->use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}