#ifndef __VERTEX_LAYOUT_H__
#define __VERTEX_LAYOUT_H__

#include "common.h"

CLASS_PTR(VertexLayout)
class VertexLayout
{
public:
    static VertexLayoutUniquePtr create();
    ~VertexLayout();

    uint32_t get() const { return m_vertex_array_obj; }
    void bind() const;
    void set_attrib(uint32_t attrib_idx, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset) const;
    void disable_attrib(int attrib_idx) const;

private:
    VertexLayout() {}
    void init();
    uint32_t m_vertex_array_obj { 0 };
};

#endif // __VERTEX_LAYOUT_H__