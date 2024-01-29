#include "shader.h"

Shader::~Shader()
{
    if (m_shader) 
    {
        glDeleteShader(m_shader);
    }
}

ShaderUniquePtr Shader::create_from_file(const std::string& file_name, GLenum shader_type)
{
    // shader 객체를 스마트 포인트로 생성
    auto shader = ShaderUniquePtr(new Shader());

    // shader 객체의 file load가 실패할 경우 nullptr 반환
    if (!shader->load_file(file_name, shader_type)) return nullptr;

    // 스마트 포인터 명의를 이동
    return std::move(shader);
}

bool Shader::load_file(const std::string& file_name, GLenum shader_type)
{
    // 파일 이름으로 문자열을 할당
    auto result_str = load_textfile(file_name);

    // result_str 에 값이 없다면 false를 반환
    if (!result_str.has_value()) return false;

    // 파일로부터 읽어온 값, 주소, 길이를 할당
    auto& code_value = result_str.value();
    const char* code_ptr = code_value.c_str();
    int32_t code_length = (int32_t)code_value.length();

    // shader 생성 및 컴파일
    m_shader = glCreateShader(shader_type);
    glShaderSource(m_shader, 1, (const GLchar* const*)&code_ptr, &code_length);
    glCompileShader(m_shader);

    // 컴파일 체크
    int is_sucess;
    char info_log[1024];
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &is_sucess);
    if (!is_sucess)
    {
        glGetShaderInfoLog(m_shader, 1024, NULL, info_log);
        SPDLOG_ERROR("failed create fragment shader: {}", info_log);
        return false;
    }
    return true;
}