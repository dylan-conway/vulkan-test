#ifndef GVEC_H
#define GVEC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <vulkan/vulkan.h>

#include "dc_utils.h"

typedef struct generic_vector* gvec;

#define GVEC_TYPE(t) gvec_##t
enum gvec_type
{
    gvec_unassigned = 0,
    GVEC_TYPE(i8),
    GVEC_TYPE(i16),
    GVEC_TYPE(i32),
    GVEC_TYPE(i64),
    GVEC_TYPE(u8),
    GVEC_TYPE(u16),
    GVEC_TYPE(u32),
    GVEC_TYPE(u64),
    GVEC_TYPE(f32),
    GVEC_TYPE(f64),
    GVEC_TYPE(str),
    GVEC_TYPE(VkExtent2D),
    GVEC_TYPE(VkRect2D),
    GVEC_TYPE(VkViewport),
};

gvec gvec_init(enum gvec_type type);
void gvec_free(gvec vec);
void gvec_print(gvec vec, const char* description);
enum gvec_type gvec_type(gvec vec);
size_t gvec_size(gvec vec);

#define GVEC_APPEND_TYPE(t) t: gvec_append_##t,
#define gvec_append(vec, value) \
    _Generic((value), \
        GVEC_APPEND_TYPE(i8)\
        GVEC_APPEND_TYPE(i16)\
        GVEC_APPEND_TYPE(i32)\
        GVEC_APPEND_TYPE(i64)\
        GVEC_APPEND_TYPE(u8)\
        GVEC_APPEND_TYPE(u16)\
        GVEC_APPEND_TYPE(u32)\
        GVEC_APPEND_TYPE(u64)\
        GVEC_APPEND_TYPE(f32)\
        GVEC_APPEND_TYPE(f64)\
        GVEC_APPEND_TYPE(str)\
        GVEC_APPEND_TYPE(VkExtent2D)\
        GVEC_APPEND_TYPE(VkRect2D)\
        GVEC_APPEND_TYPE(VkViewport)\
        default: gvec_append_error)(vec, value)

#define GVEC_APPEND_DECL(t) void gvec_append_##t(gvec vec, t value)
void gvec_append_error(void);
GVEC_APPEND_DECL(i8);
GVEC_APPEND_DECL(i16);
GVEC_APPEND_DECL(i32);
GVEC_APPEND_DECL(i64);
GVEC_APPEND_DECL(u8);
GVEC_APPEND_DECL(u16);
GVEC_APPEND_DECL(u32);
GVEC_APPEND_DECL(u64);
GVEC_APPEND_DECL(f32);
GVEC_APPEND_DECL(f64);
GVEC_APPEND_DECL(str);
GVEC_APPEND_DECL(VkExtent2D);
GVEC_APPEND_DECL(VkRect2D);
GVEC_APPEND_DECL(VkViewport);

#define GVEC_DATA_TYPE(t) t**: gvec_data_##t,
#define gvec_data(vec, data_out) \
    _Generic((data_out), \
        GVEC_DATA_TYPE(i8)\
        GVEC_DATA_TYPE(i16)\
        GVEC_DATA_TYPE(i32)\
        GVEC_DATA_TYPE(i64)\
        GVEC_DATA_TYPE(u8)\
        GVEC_DATA_TYPE(u16)\
        GVEC_DATA_TYPE(u32)\
        GVEC_DATA_TYPE(u64)\
        GVEC_DATA_TYPE(f32)\
        GVEC_DATA_TYPE(f64)\
        GVEC_DATA_TYPE(str)\
        GVEC_DATA_TYPE(VkExtent2D)\
        GVEC_DATA_TYPE(VkRect2D)\
        GVEC_DATA_TYPE(VkViewport)\
        default: gvec_data_error)(vec, data_out)

#define GVEC_DATA_DECL(t) void gvec_data_##t(gvec vec, t** data_out)
void gvec_data_error(void);
GVEC_DATA_DECL(i8);
GVEC_DATA_DECL(i16);
GVEC_DATA_DECL(i32);
GVEC_DATA_DECL(i64);
GVEC_DATA_DECL(u8);
GVEC_DATA_DECL(u16);
GVEC_DATA_DECL(u32);
GVEC_DATA_DECL(u64);
GVEC_DATA_DECL(f32);
GVEC_DATA_DECL(f64);
GVEC_DATA_DECL(str);
GVEC_DATA_DECL(VkExtent2D);
GVEC_DATA_DECL(VkRect2D);
GVEC_DATA_DECL(VkViewport);


#endif