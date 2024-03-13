#include "context.h"
#include "image.h"

#include <imgui.h>

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
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    };

    uint32_t indices[] = {
        0,  2,  1,  2,  0,  3,
        4,  5,  6,  6,  7,  4,
        8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };

    // create vertex object
    m_vertex_layout = VertexLayout::create();

    // create vertex array object
    m_vertex_buffer = Buffer::create_with_data(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertex_data, sizeof(float) * 120);

    // set position attribute
    m_vertex_layout->set_attrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    m_vertex_layout->set_attrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);

    // create index buffer
    m_index_buffer = Buffer::create_with_data(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);

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

    auto image2 = Image::load("./resources/textures/awesomeface.png");
    if (!image2) return false;
    SPDLOG_INFO("image2: {}x{}, {} channels", image2->get_width(), image2->get_height(), image2->get_channel_cnt());

    texture = Texture::create_from_image(image.get());
    texture2 = Texture::create_from_image(image2.get());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2->get());

    m_shader_program->use();
    m_shader_program->set_uniform("texture_obj", 0);
    m_shader_program->set_uniform("texture_obj2", 1);

    return true;
}

void Context::input(GLFWwindow* window)
{
    if(!camera_control) return;

    const float camera_speed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera_pos += camera_speed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera_pos -= camera_speed * camera_front;

    auto camera_right = glm::normalize(glm::cross(camera_up, -camera_front));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera_pos += camera_speed * camera_right;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera_pos -= camera_speed * camera_right;

    auto camera_up = glm::normalize(glm::cross(-camera_front, camera_right));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera_pos += camera_speed * camera_up;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera_pos -= camera_speed * camera_up;
}

void Context::render()
{
    if (ImGui::Begin("my first ImGui window")) {
        if (ImGui::ColorEdit4("clear color", glm::value_ptr(clear_color)))
        {
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        }
        ImGui::Separator();
        ImGui::DragFloat3("camera pos", glm::value_ptr(camera_pos), 0.01f);
        ImGui::DragFloat("camera yaw", &camera_yaw, 0.5f);
        ImGui::DragFloat("camera pitch", &camera_pitch, 0.5f, -89.0f, 89.0f);
        ImGui::Separator();
        if (ImGui::Button("reset camera")) 
        {
            camera_yaw = 0.0f;
            camera_pitch = 0.0f;
            camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
        }
    }
    ImGui::End();

    std::vector<glm::vec3> cube_position = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    m_shader_program->use();

    camera_front = 
        glm::rotate(glm::mat4(1.0f), glm::radians(camera_yaw), glm::vec3(0.0f, 1.0f, 0.0f)) * 
        glm::rotate(glm::mat4(1.0f), glm::radians(camera_pitch), glm::vec3(1.0f, 0.0f, 0.0f)) * 
        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    
    auto projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.01f, 50.0f);

    float angle = glfwGetTime() * glm::pi<float>() * 0.5f;
    auto x = sinf(angle) * 10.0f;
    auto z = cosf(angle) * 10.0f;

    auto view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

    for (size_t i = 0; i < cube_position.size(); i++)
    {
        auto& pos = cube_position[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model, glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i), glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        m_shader_program->set_uniform("transform", transform);
    
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}

void Context::reshape(int width, int height)
{
    this->width = width;
    this->height = height;
    glViewport(0, 0, this->width, this->height);
}

void Context::mouse_move(double x, double y)
{
    if(!camera_control) return;
    
    static glm::vec2 prev_position = glm::vec2((float)x, (float)y);
    auto position = glm::vec2((float)x, (float)y);
    auto delta_position = position - prev_position;

    const float camera_rot_speed = 0.8f;
    camera_yaw -= delta_position.x * camera_rot_speed;
    camera_pitch -= delta_position.y * camera_rot_speed;

    if (camera_yaw < 0.0f) camera_yaw += 360.0f;
    if (camera_yaw > 360.0f) camera_yaw -= 360.0f;

    if (camera_pitch > 89.0f) camera_pitch = 89.0f;
    if (camera_pitch < -89.0f) camera_pitch = -89.0f;

    prev_position = position;
}

void Context::mouse_click(int button, int action, double x, double y)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT) // 눌린 마우스의 버튼이 오른쪽 키일 경우
    {
        if (action == GLFW_PRESS)
        {   
            prev_mouse_position = glm::vec2((float)x, (float)y);
            camera_control = true;
        }
        else if (action == GLFW_RELEASE)
        {
            camera_control = false;
        }
    }
}