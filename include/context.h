#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "shader_program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "image.h"

CLASS_PTR(Context)
class Context
{
public:
    static ContextUniquePtr create();
    void render();

private:
    Context() {}
    bool init();

private:
    ShaderProgramUniquePtr m_shader_program;
    VertexLayoutUniquePtr m_vertex_layout; // vertex array object
    BufferUniquePtr m_vertex_buffer;
    BufferUniquePtr m_index_buffer;

    uint32_t texture;
};

#endif // __CONTEXT_H__