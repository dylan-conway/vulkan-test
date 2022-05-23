#ifndef DVEC_H
#define DVEC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <vulkan/vulkan.h>

#include "dc_utils.h"

#define DVEC_TYPE_DECL(T)\
    typedef struct dynamic_vector_##T* dvec_##T;\
    dvec_##T dvec_init_##T(void);\
    void dvec_free_##T(dvec_##T dvec);\
    T* dvec_data_##T(dvec_##T dvec);\
    size_t dvec_size_##T(dvec_##T dvec);\
    void dvec_append_##T(dvec_##T dvec, T value)\

DVEC_TYPE_DECL(i8);
DVEC_TYPE_DECL(i16);
DVEC_TYPE_DECL(i32);
DVEC_TYPE_DECL(i64);
DVEC_TYPE_DECL(u8);
DVEC_TYPE_DECL(u16);
DVEC_TYPE_DECL(u32);
DVEC_TYPE_DECL(u64);
DVEC_TYPE_DECL(f32);
DVEC_TYPE_DECL(f64);
DVEC_TYPE_DECL(str);
DVEC_TYPE_DECL(VkRect2D);

#define dvec_init(type) dvec_init_##type();

#define DVEC_CASE(method, type) dvec_##type: dvec_##method##_##type

#define dvec_free(dvec) _Generic((dvec),\
    DVEC_CASE(free, i8),\
    DVEC_CASE(free, i16),\
    DVEC_CASE(free, i32),\
    DVEC_CASE(free, i64),\
    DVEC_CASE(free, u8),\
    DVEC_CASE(free, u16),\
    DVEC_CASE(free, u32),\
    DVEC_CASE(free, u64),\
    DVEC_CASE(free, f32),\
    DVEC_CASE(free, f64),\
    DVEC_CASE(free, str),\
    DVEC_CASE(free, VkRect2D))\
    (dvec)

#define dvec_data(dvec) _Generic((dvec),\
    DVEC_CASE(data, i8),\
    DVEC_CASE(data, i16),\
    DVEC_CASE(data, i32),\
    DVEC_CASE(data, i64),\
    DVEC_CASE(data, u8),\
    DVEC_CASE(data, u16),\
    DVEC_CASE(data, u32),\
    DVEC_CASE(data, u64),\
    DVEC_CASE(data, f32),\
    DVEC_CASE(data, f64),\
    DVEC_CASE(data, str),\
    DVEC_CASE(data, VkRect2D))\
    (dvec)

#define dvec_size(dvec) _Generic((dvec),\
    DVEC_CASE(size, i8),\
    DVEC_CASE(size, i16),\
    DVEC_CASE(size, i32),\
    DVEC_CASE(size, i64),\
    DVEC_CASE(size, u8),\
    DVEC_CASE(size, u16),\
    DVEC_CASE(size, u32),\
    DVEC_CASE(size, u64),\
    DVEC_CASE(size, f32),\
    DVEC_CASE(size, f64),\
    DVEC_CASE(size, str),\
    DVEC_CASE(size, VkRect2D))\
    (dvec)

#define dvec_append(dvec, value) _Generic((dvec),\
    DVEC_CASE(append, i8),\
    DVEC_CASE(append, i16),\
    DVEC_CASE(append, i32),\
    DVEC_CASE(append, i64),\
    DVEC_CASE(append, u8),\
    DVEC_CASE(append, u16),\
    DVEC_CASE(append, u32),\
    DVEC_CASE(append, u64),\
    DVEC_CASE(append, f32),\
    DVEC_CASE(append, f64),\
    DVEC_CASE(append, str),\
    DVEC_CASE(append, VkRect2D))\
    (dvec, value)

#endif