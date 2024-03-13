#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "shader_program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "image.h"
#include "texture.h"

CLASS_PTR(Context)
class Context
{
public:
    static ContextUniquePtr create();
    void render();
    void input(GLFWwindow* window);
    void reshape(int width, int height);
    void mouse_move(double x, double y);
    void mouse_click(int button, int action, double x, double y);

private:
    Context() {}
    bool init();

private:
    // clear parameter
    glm::vec4 clear_color { glm::vec4(0.1f, 0.2f, 0.3f, 0.0f) };

    ShaderProgramUniquePtr m_shader_program;
    VertexLayoutUniquePtr m_vertex_layout; // vertex array object
    BufferUniquePtr m_vertex_buffer;
    BufferUniquePtr m_index_buffer;
    TextureUniquePtr texture;
    TextureUniquePtr texture2;

    // camera parameter
    glm::vec3 camera_pos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3 camera_front { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 camera_up { glm::vec3(0.0f, 1.0f, 0.0f) };

    // shape parameter
    int width { WINDOW_WIDTH };
    int height { WINDOW_HEIGHT };

    // mouse parameter
    float camera_pitch { 0.0f };
    float camera_yaw { 0.0f };
    bool camera_control { false };
    glm::vec2 prev_mouse_position { glm::vec2(0.0f) };
};

#endif // __CONTEXT_H__