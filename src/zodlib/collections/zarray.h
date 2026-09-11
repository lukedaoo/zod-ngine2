#pragma once

#include "../precompiled.h"

template <class _type_, usize _number_elements_>
class zArray;

template <class _type_, int _dim1_, int _dim2_>
class zArray2D;

template <class _type_, usize _number_elements_>
class zReadOnlyArray;

template <class _type_, usize _number_elements_>
class zArray {
    static_assert(_number_elements_ > 0,
                  "Number of elements must be greater than zero");
private:
    _type_ m_elements[_number_elements_];
public:
    constexpr int num() const { return _number_elements_; }

    constexpr int byte_size() const { return sizeof(m_elements); }

    void zero() { memset((void*)m_elements, ZERO_MEMORY, sizeof(m_elements)); }

    void fill(const char value) {
        memset((void*)m_elements, value, sizeof(m_elements));
    }

    constexpr const _type_& operator[](int index) const {
        assert((unsigned)index < _number_elements_);
        return m_elements[index];
    }

    constexpr _type_& operator[](int index) {
        assert((unsigned)index < _number_elements_);
        return m_elements[index];
    }

    // Not a copy - changes in zArray show up in the zReadOnlyArray
    // e.g. arr[0] = 1; auto v = arr.to_readonly(); arr[0] = 2; v[0] == 2
    constexpr zReadOnlyArray<_type_, _number_elements_> to_readonly() const {
        return zReadOnlyArray<_type_, _number_elements_>(*this);
    }
};

#ifndef ARRAY_COUNT
#define ARRAY_COUNT(array_name) (sizeof(array_name) / sizeof((array_name)[0]))
#endif

#ifndef ARRAY_DEF
#define ARRAY_DEF(array_name) array_name, ARRAY_COUNT(array_name)
#endif

template <class _type_, int _dim1_, int _dim2_>
class zArray2D {
    static_assert(_dim1_ > 0 && _dim2_ > 0,
                  "Dimensions must be greater than zero");
private:
    zArray<zArray<_type_, _dim2_>, _dim1_> m_grid;
public:
    constexpr int num_rows() const { return _dim1_; }
    constexpr int num_cols() const { return _dim2_; }

    constexpr int byte_size() const { return m_grid.byte_size(); }

    void          zero() { m_grid.zero(); }

    constexpr const zArray<_type_, _dim2_>& operator[](int index) const {
        assert((unsigned)index < _dim1_);
        return m_grid[index];
    }
    constexpr zArray<_type_, _dim2_>& operator[](int index) {
        assert((unsigned)index < _dim1_);
        return m_grid[index];
    }
};

template <class _type_, usize _number_elements_>
class zReadOnlyArray {
private:
    const _type_* m_data;
public:
    constexpr zReadOnlyArray(const _type_* data) : m_data(data) {}

    constexpr zReadOnlyArray(const zArray<_type_, _number_elements_>& other)
        : m_data(&other[0]) {}

    constexpr const _type_& operator[](int index) const {
        assert((unsigned)index < _number_elements_);
        return m_data[index];
    }

    constexpr int num() const { return _number_elements_; }
};
