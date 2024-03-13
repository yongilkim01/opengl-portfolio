#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "common.h"

CLASS_PTR(Image)
class Image
{
public:
    static ImageUniquePtr load(const std::string& file_path);
    static ImageUniquePtr create(int width, int height, int channel_cnt = 4);
    ~Image();

    const uint8_t* get_data() const { return data; }
    int get_width() const { return width; }
    int get_height() const { return height; }
    int get_channel_cnt() const { return channel_cnt; }

    void set_check_image(int grid_x, int grid_y);

private:
    Image() {};
    bool load_use_stb(const std::string& file_path);
    bool allocate(int width, int height, int channel_cnt);
    int width { 0 };
    int height { 0 };
    int channel_cnt { 0 };
    unsigned char * data { nullptr };
};

#endif // __IMAGE_H__