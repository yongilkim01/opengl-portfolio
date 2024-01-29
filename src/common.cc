#include "common.h"
#include <fstream>
#include <sstream>

std::optional<std::string> load_textfile(const std::string& file_name)
{
    // 파일을 읽어올 객체
    std::ifstream fin(file_name);

    // 파일이 제대로 열리지 않았을 경우
    if (!fin.is_open())
    {
        SPDLOG_ERROR("failed to open file: {}", file_name);
        return {};
    }

    // 파일 내용을 저장하기 위한 stringstream 객체 생성
    std::stringstream text;

    // 파일 내용을 stringstream에 저장
    text << fin.rdbuf();

    // 문자열 반환
    return text.str();
}

