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

ImageUniquePtr Image::create(int width, int height, int channel_cnt)
{
    // 객체 생성
    auto image = ImageUniquePtr(new Image());

    // 할당 실패시 nullptr 반환
    if (!image->allocate(width, height, channel_cnt))
        return nullptr;
    
    //소유권 반환
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
    // OpenGL은 좌하단이 원점, 일반 이미지는 좌상단이 원점
    stbi_set_flip_vertically_on_load(true);
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

/*///////////////////////////////////////////////////////
grid_x = 4
normal  0 1 2 3 4 5
convert 0 0 0 0 1 1
*////////////////////////////////////////////////////////
void Image::set_check_image(int grid_x, int grid_y)
{
    for (int j = 0; j < height; j++) 
    {
        for (int i = 0; i < width; i++)
        {
            int pos = (j * width + i) * channel_cnt;
            bool even = ((i / grid_x) + (j / grid_y)) % 2 == 0;
            uint8_t value = even ? 255 : 0;
            for (int k = 0; k < channel_cnt; k++)
                data[pos + k] = value;
            if(channel_cnt > 3)
                data[3] = 255;
        }
    }
}

bool Image::allocate(int width, int height, int channel_cnt)
{
    // 멤버변수 초기화
    this->width = width;
    this->height = height;
    this->channel_cnt = channel_cnt;
    data = (uint8_t*)malloc(width * height * channel_cnt);

    // 할당 결과 반환
    return data ? true : false;
}