#include "texture.h"

TextureUniquePtr Texture::create_from_image(const Image* image)
{
    // 객체 생성
    auto texture = TextureUniquePtr(new Texture());
    texture->create_texture();

    // 텍스처의 이미지 설정
    texture->set_texture_from_image(image);

    // 소유권 반환
    return std::move(texture);
}

Texture::~Texture()
{
    if (!texture)
        glDeleteTextures(1, &texture);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::set_filter(uint32_t min_filter, uint32_t mag_filter) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
}

void Texture::set_filter(uint32_t min_filter, uint32_t mag_filter) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
}

void Texture::set_wrap(uint32_t s_wrap, uint32_t t_wrap) const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_wrap);
}

void Texture::create_texture()
{
    glGenTextures(1, &texture);
    bind();
    set_filter(GL_LINEAR, GL_LINEAR);
    set_wrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void Texture::set_texture_from_image(const Image* image)
{
    GLenum format = GL_RGBA;
    switch(image->get_channel_cnt())
    {
    default: break;
    case1:
        format = GL_RED; 
        break;
    case2:
        format = GL_RG;
        break;
    case3:
        format = GL_RGB;
        break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->get_width(), 
        image->get_height(), 0, format, GL_UNSIGNED_BYTE, image->get_data());
}