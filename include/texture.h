#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "image.h"

CLASS_PTR(Texture)
class Texture 
{
private:
    Texture() {}
public:
    ~Texture();

////////////////////////// create function //////////////////////////
private:
    void create_texture();
    void set_texture_from_image(const Image* image);
public:
    static TextureUniquePtr create_from_image(const Image* image);
    void bind() const;

/////////////////////////////// get set ///////////////////////////////
private:
    void set_texture_from_image(const Image* image);
public:
    const uint32_t get() const { return texture; }
    void set_filter(uint32_t min_filter, uint32_t mag_filter) const;
    void set_wrap(uint32_t s_wrap, uint32_t t_wrap) const;

private: // variable
    uint32_t texture { 0 };
};

#endif // __TEXTURE_H__