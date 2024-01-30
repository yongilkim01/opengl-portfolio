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
    if (m_shader_program)
    {
        glDeleteProgram(m_shader_program);
    }
}

bool ShaderProgram::link(const std::vector<ShaderSharedPtr>& shaders)
{
    // opengl 로부터 shader program 생성
    m_shader_program = glCreateProgram();

    // 입력으로 전달받은 shader의 수(vectex, fragment)만큼 반복하여 shader program에 부착
    for (auto& shader : shaders)
        glAttachShader(m_shader_program, shader->get());

    // shader program 연결
    glLinkProgram(m_shader_program);

    // 컴파일 체크
    int is_sucess;

    // shader program으로부터 shader 정보를 탐색하여 성공 여부를 is_sucess에 반환
    glGetProgramiv(m_shader_program, GL_LINK_STATUS, &is_sucess);

    if (!is_sucess)
    {
        char info_log[1024];
        glGetProgramInfoLog(m_shader_program, 1024, nullptr, info_log);
        SPDLOG_ERROR("failed to link program: {}", info_log);
        return false;
    }
    return true;
}