#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include "common.h"
#include "shader.h"

CLASS_PTR(ShaderProgram)
class ShaderProgram
{
public:
    static ShaderProgramUniquePtr create(const std::vector<ShaderSharedPtr>& shaders);
    ~ShaderProgram();
    uint32_t get() const { return shader_program; }
    void use() const;

    void set_uniform(const std::string& name, int value) const;
    void set_uniform(const std::string& name, const glm::mat4& value) const;
    void set_uniform(const std::string& name, float value) const;
    void set_uniform(const std::string& name, const glm::vec3& value) const;

private:
    ShaderProgram() {}
    bool link(const std::vector<ShaderSharedPtr>& shaders);

private:
    uint32_t shader_program {0};
};

#endif // __SHADER_PROGRAM_H__