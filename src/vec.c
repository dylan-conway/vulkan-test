#include "vec.h"

#define VEC_TYPE_DEFN(T)\
\
    struct vector_##T\
    {\
        size_t capacity;\
        T* data;\
    };\
\
    vec_##T vec_init_##T(size_t capacity)\
    {\
        if (0 == capacity) return NULL;\
        vec_##T vec = calloc(1, sizeof(struct vector_##T));\
        vec->capacity = capacity;\
        vec->data = calloc(vec->capacity, sizeof(T));\
        return vec;\
    }\
\
    void vec_free_##T(vec_##T vec)\
    {\
        if (NULL == vec) return;\
        free(vec->data);\
        vec->capacity = 0;\
        free(vec);\
    }\
\
    T* vec_data_##T(vec_##T vec)\
    {\
        return vec->data;\
    }\
\
    size_t vec_capacity_##T(vec_##T vec)\
    {\
        return vec->capacity;\
    }\
\
    void vec_resize_##T(vec_##T vec, size_t capacity)\
    {\
        vec->capacity = capacity;\
        vec->data = realloc(vec->data, sizeof(vec->data[0]) * capacity);\
    }

VEC_TYPE_DEFN(i8)
VEC_TYPE_DEFN(i16)
VEC_TYPE_DEFN(i32)
VEC_TYPE_DEFN(i64)
VEC_TYPE_DEFN(u8)
VEC_TYPE_DEFN(u16)
VEC_TYPE_DEFN(u32)
VEC_TYPE_DEFN(u64)
VEC_TYPE_DEFN(f32)
VEC_TYPE_DEFN(f64)
VEC_TYPE_DEFN(str)
VEC_TYPE_DEFN(VkRect2D)