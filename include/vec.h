#ifndef VEC_H
#define VEC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <vulkan/vulkan.h>

#include "dc_utils.h"

#define VEC_TYPE_DECL(T)\
    typedef struct vector_##T* vec_##T;\
    vec_##T vec_init_##T(size_t capacity);\
    void vec_free_##T(vec_##T vec);\
    T* vec_data_##T(vec_##T vec);\
    size_t vec_capacity_##T(vec_##T vec);\
    void vec_resize_##T(vec_##T vec, size_t capacity)\

VEC_TYPE_DECL(i8);
VEC_TYPE_DECL(i16);
VEC_TYPE_DECL(i32);
VEC_TYPE_DECL(i64);
VEC_TYPE_DECL(u8);
VEC_TYPE_DECL(u16);
VEC_TYPE_DECL(u32);
VEC_TYPE_DECL(u64);
VEC_TYPE_DECL(f32);
VEC_TYPE_DECL(f64);
VEC_TYPE_DECL(str);
VEC_TYPE_DECL(VkRect2D);

#define VEC(type, var_name) vec_##type

#define vec_init(type, capacity) vec_init_##type(capacity);

#define VEC_METHOD_CASE(method, type) vec_##type: vec_##method##_##type

#define vec_free(vec) _Generic((vec),\
    VEC_METHOD_CASE(free, i8),\
    VEC_METHOD_CASE(free, i16),\
    VEC_METHOD_CASE(free, i32),\
    VEC_METHOD_CASE(free, i64),\
    VEC_METHOD_CASE(free, u8),\
    VEC_METHOD_CASE(free, u16),\
    VEC_METHOD_CASE(free, u32),\
    VEC_METHOD_CASE(free, u64),\
    VEC_METHOD_CASE(free, f32),\
    VEC_METHOD_CASE(free, f64),\
    VEC_METHOD_CASE(free, str),\
    VEC_METHOD_CASE(free, VkRect2D))\
    (vec)

#define vec_data(vec) _Generic((vec),\
    VEC_METHOD_CASE(data, i8),\
    VEC_METHOD_CASE(data, i16),\
    VEC_METHOD_CASE(data, i32),\
    VEC_METHOD_CASE(data, i64),\
    VEC_METHOD_CASE(data, u8),\
    VEC_METHOD_CASE(data, u16),\
    VEC_METHOD_CASE(data, u32),\
    VEC_METHOD_CASE(data, u64),\
    VEC_METHOD_CASE(data, f32),\
    VEC_METHOD_CASE(data, f64),\
    VEC_METHOD_CASE(data, str),\
    VEC_METHOD_CASE(data, VkRect2D))\
    (vec)

#define vec_capacity(vec) _Generic((vec),\
    VEC_METHOD_CASE(capacity, i8),\
    VEC_METHOD_CASE(capacity, i16),\
    VEC_METHOD_CASE(capacity, i32),\
    VEC_METHOD_CASE(capacity, i64),\
    VEC_METHOD_CASE(capacity, u8),\
    VEC_METHOD_CASE(capacity, u16),\
    VEC_METHOD_CASE(capacity, u32),\
    VEC_METHOD_CASE(capacity, u64),\
    VEC_METHOD_CASE(capacity, f32),\
    VEC_METHOD_CASE(capacity, f64),\
    VEC_METHOD_CASE(capacity, str),\
    VEC_METHOD_CASE(capacity, VkRect2D))\
    (vec)

#define vec_resize(vec, capacity) _Generic((vec),\
    VEC_METHOD_CASE(resize, i8),\
    VEC_METHOD_CASE(resize, i16),\
    VEC_METHOD_CASE(resize, i32),\
    VEC_METHOD_CASE(resize, i64),\
    VEC_METHOD_CASE(resize, u8),\
    VEC_METHOD_CASE(resize, u16),\
    VEC_METHOD_CASE(resize, u32),\
    VEC_METHOD_CASE(resize, u64),\
    VEC_METHOD_CASE(resize, f32),\
    VEC_METHOD_CASE(resize, f64),\
    VEC_METHOD_CASE(resize, str),\
    VEC_METHOD_CASE(resize, VkRect2D))\
    (vec, capacity)

#endif