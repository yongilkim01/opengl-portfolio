#ifndef __SHADER_H__
#define __SHADER_H__

#include "common.h"

CLASS_PTR(Shader);
class Shader
{
private:
    Shader() {}
    bool load_file(const std::string& file_name, GLenum shader_type);
public:
    static ShaderUniquePtr create_from_file(const std::string& file_name, GLenum shader_type);
    ~Shader();
    uint32_t get() const { return m_shader; }

private:
    uint32_t m_shader {0};
};

#endif // __SHADER_H__