#include "buffer.h"

BufferUniquePtr Buffer::create_with_data(uint32_t buffer_type, uint32_t usage, const void* data, size_t data_size)
{
    // buffer 객체 생성
    auto buffer = BufferUniquePtr(new Buffer());

    // 객체 생성 실패시 nullptr 반환
    if(!buffer->init(buffer_type, usage, data, data_size))
        return nullptr;
    
    // 주솟값 이동
    return std::move(buffer);
}

Buffer::~Buffer()
{
    if(m_buffer)
    {
        glDeleteBuffers(1, &m_buffer);
    }
}

void Buffer::bind() const
{
    glBindBuffer(m_buffer_type, m_buffer);
}

bool Buffer::init(uint32_t buffer_type, uint32_t usage, const void* data, size_t data_size)
{
    m_buffer_type = buffer_type;
    m_usage = usage;
    glGenBuffers(1, &m_buffer);
    bind();
    glBufferData(m_buffer_type, data_size, data, usage);
    return true;
}
