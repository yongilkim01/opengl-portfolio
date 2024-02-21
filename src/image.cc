#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

ImageUniquePtr Image::load(const std::string& file_path)
{
    // 객체 생성
    auto image = ImageUniquePtr(new Image());

    // 이미지 불러오기 실패시 nullptr 반환
    if(!image->load_use_stb(file_path))
        return nullptr;
    
    // 주솟값 반환
    return std::move(image);
}

Image::~Image()
{
    // data의 값이 존재한다면 stb라이브러리를 사용하여 메모리 해제
    if (data)
        stbi_image_free(data);
}

bool Image::load_use_stb(const std::string& file_path)
{
    // data에 경로에 있는 이미지 데이터를 할당
    data = stbi_load(file_path.c_str(), &width, &height, &channel_cnt, 0);

    // data의 메모리 공간에 아무 값도 없다면 에러 출력
    if (!data)
    {
        SPDLOG_ERROR("failed to load image: {}", file_path);
        return false;
    }

    // 성공 반환
    return true;
}