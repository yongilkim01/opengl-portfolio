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
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3   
    };

    // create vertex object
    m_vertex_layout = VertexLayout::create();

    // create vertex array object
    m_vertex_buffer = Buffer::create_with_data(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertex_data, sizeof(float) * 32);

    // set position attribute
    m_vertex_layout->set_attrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    m_vertex_layout->set_attrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    m_vertex_layout->set_attrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);


    // create index buffer
    m_index_buffer = Buffer::create_with_data(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    // create shader
    ShaderSharedPtr vertex_shader = Shader::create_from_file("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderSharedPtr fragment_shader = Shader::create_from_file("./shader/texture.fs", GL_FRAGMENT_SHADER);

    if(!vertex_shader || !fragment_shader) return false;

    SPDLOG_INFO("vertex shader id: {}", vertex_shader->get());
    SPDLOG_INFO("fragment shader id: {}", fragment_shader->get());

    // create shader program
    m_shader_program = ShaderProgram::create({fragment_shader, vertex_shader});


    if(!vertex_shader || !fragment_shader) return false;

    SPDLOG_INFO("shader program id: {}", m_shader_program->get());

    // clear background color
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    // create image object
    auto image = Image::load("./resources/textures/container.jpg");
    if (!image) return false;
    SPDLOG_INFO("image: {}x{}, {} channels", image->get_width(), image->get_height(), image->get_channel_cnt());

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->get_width(), image->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->get_data());

    return true;
}

void Context::render()
{
    glClear(GL_COLOR_BUFFER_BIT);;
    m_shader_program->use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}