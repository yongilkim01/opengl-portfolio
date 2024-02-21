#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "common.h"

CLASS_PTR(Image)
class Image
{
public:
    static ImageUniquePtr load(const std::string& file_path);
    ~Image();

    const uint8_t* get_data() const { return data; }
    int get_width() const { return width; }
    int get_height() const { return height; }
    int get_channel_cnt() const { return channel_cnt; }

private:
    Image() {};
    bool load_use_stb(const std::string& file_path);
    int width { 0 };
    int height { 0 };
    int channel_cnt { 0 };
    uint8_t* data { nullptr };
};

#endif // __IMAGE_H__