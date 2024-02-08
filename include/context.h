#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "shader_program.h"

CLASS_PTR(Context)
class Context
{
public:
    static ContextUniquePtr create();
    void render();

private:
    Context() {}
    bool init();
    ShaderProgramUniquePtr m_shader_program;
};

#endif // __CONTEXT_H__