#include "vertex_layout.h"

VertexLayoutUniquePtr VertexLayout::create()
{
    // vertex layout 객체 생성
    auto vertex_layout = VertexLayoutUniquePtr(new VertexLayout());

    // 객체 초기화
    vertex_layout->init();

    // 주솟값 이동 허용
    return std::move(vertex_layout);
}

VertexLayout::~VertexLayout()
{
    if (m_vertex_array_obj)
        glDeleteVertexArrays(1, &m_vertex_array_obj);
}

void VertexLayout::bind() const
{
    glBindVertexArray(m_vertex_array_obj);
}

void VertexLayout::set_attrib(uint32_t attrib_idx, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset) const
{
    glEnableVertexAttribArray(attrib_idx);
    glVertexAttribPointer(attrib_idx, count, type, normalized, stride, (const void*)offset);
}

void VertexLayout::disable_attrib(int attrib_idx) const
{

}

void VertexLayout::init()
{
    glGenVertexArrays(1, &m_vertex_array_obj);
    bind();
}