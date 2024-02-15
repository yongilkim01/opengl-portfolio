#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "common.h"

CLASS_PTR(Buffer)
class Buffer 
{
public:
    static BufferUniquePtr create_with_data(uint32_t buffer_type, uint32_t usage, const void* data, size_t data_size);
    ~Buffer();
    uint32_t get() const { return m_buffer; }
    void bind() const;

private:
    Buffer() {}
    bool init(uint32_t buffer_type, uint32_t usage, const void* data, size_t data_size);
    uint32_t m_buffer { 0 };
    uint32_t m_buffer_type { 0 };
    uint32_t m_usage { 0 };
};

#endif // __BUFFER_H__