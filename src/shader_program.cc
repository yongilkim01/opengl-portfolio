#include "shader_program.h"

ShaderProgramUniquePtr ShaderProgram::create(const std::vector<ShaderSharedPtr>& shaders)
{
    // shader program 객체 생성
    auto shader_program = ShaderProgramUniquePtr(new ShaderProgram());

    // shader program의 연결이 실패하면 null 반환
    if (!shader_program->link(shaders)) return nullptr;

    // shared pointer가 받을 수 있게 move 함수를 사용해서 반환
    return std::move(shader_program);
}

ShaderProgram::~ShaderProgram()
{
    if (shader_program)
    {
        glDeleteProgram(shader_program);
    }
}

bool ShaderProgram::link(const std::vector<ShaderSharedPtr>& shaders)
{
    // opengl 로부터 shader program 생성
    shader_program = glCreateProgram();

    // 입력으로 전달받은 shader의 수(vectex, fragment)만큼 반복하여 shader program에 부착
    for (auto& shader : shaders)
        glAttachShader(shader_program, shader->get());

    // shader program 연결
    glLinkProgram(shader_program);

    // 컴파일 체크
    int is_sucess;

    // shader program으로부터 shader 정보를 탐색하여 성공 여부를 is_sucess에 반환
    glGetProgramiv(shader_program, GL_LINK_STATUS, &is_sucess);

    if (!is_sucess)
    {
        char info_log[1024];
        glGetProgramInfoLog(shader_program, 1024, nullptr, info_log);
        SPDLOG_ERROR("failed to link program: {}", info_log);
        return false;
    }
    return true;
}

void ShaderProgram::use() const
{
    glUseProgram(shader_program);
}

void ShaderProgram::set_uniform(const std::string& name, int value) const
{
    auto location = glGetUniformLocation(shader_program, name.c_str());
    glUniform1i(location, value);
}

void ShaderProgram::set_uniform(const std::string& name, const glm::mat4& value) const
{
    auto location = glGetUniformLocation(shader_program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::set_uniform(const std::string& name, float value) const
{
    auto location = glGetUniformLocation(shader_program, name.c_str());
    glUniform1f(location, value);
}

void ShaderProgram::set_uniform(const std::string& name, const glm::vec3& value) const
{
    auto location = glGetUniformLocation(shader_program, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));
}