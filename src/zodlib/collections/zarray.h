#pragma once

#include "../precompiled.h"

template <class _type_, usize _number_elements_>
class zArray {
    static_assert(_number_elements_ > 0,
                  "Number of elements must be greater than zero");
private:
    _type_ m_elements[_number_elements_];
public:
    zArray() {}

    int  num() const { return _number_elements_; }

    int  byte_size() const { return sizeof(m_elements); }

    void zero() { memset(m_elements, ZERO_MEMORY, sizeof(m_elements)); }

    void fill(const char value) { memset(m_elements, value, sizeof(m_elements)); }

    const _type_& operator[](int index) const {
        assert((unsigned)index < _number_elements_);
        return m_elements[index];
    }
    _type_& operator[](int index) {
        assert((unsigned)index < _number_elements_);
        return m_elements[index];
    }
};

#ifndef ARRAY_COUNT
#define ARRAY_COUNT(array_name) (sizeof(array_name) / sizeof((array_name)[0]))
#endif

// e.g.
// void print(int* array, int count)
// it can be called as print(ARRAY_CALL(array))
#ifndef ARRAY_DEF
#define ARRAY_DEF(array_name) array_name, ARRAY_COUNT(array_name)
#endif

// @_dim1_: Number of rows
// @_dim2_: Number of columns
template <class _type_, int _dim1_, int _dim2_>
class zArray2D {
    static_assert(_dim1_ > 0 && _dim2_ > 0,
                  "Dimensions must be greater than zero");
private:
    zArray<zArray<_type_, _dim2_>, _dim1_> m_grid;
public:
    zArray2D() {}

    int                           num_rows() const { return _dim1_; }
    int                           num_cols() const { return _dim2_; }

    int                           byte_size() const { return m_grid.byte_size(); }

    void                          zero() { m_grid.zero(); }

    const zArray<_type_, _dim2_>& operator[](int index) const {
        assert((unsigned)index < _dim1_);
        return m_grid[index];
    }
    zArray<_type_, _dim2_>& operator[](int index) {
        assert((unsigned)index < _dim1_);
        return m_grid[index];
    }
};
