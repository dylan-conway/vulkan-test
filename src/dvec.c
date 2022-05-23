#include "dvec.h"

#define DVEC_TYPE_DEFN(T)\
    struct dynamic_vector_##T\
    {\
        size_t size;\
        size_t capacity;\
        T* data;\
    };\
\
    dvec_##T dvec_init_##T(void)\
    {\
        dvec_##T dvec = calloc(1, sizeof(struct dynamic_vector_##T));\
        dvec->size = 0;\
        dvec->capacity = 2;\
        dvec->data = calloc(dvec->capacity, sizeof(T));\
        return dvec;\
    }\
\
    void dvec_free_##T(dvec_##T dvec)\
    {\
        if (NULL == dvec) return;\
        free(dvec->data);\
        dvec->size = 0;\
        free(dvec);\
    }\
\
    T* dvec_data_##T(dvec_##T dvec)\
    {\
        return dvec->data;\
    }\
\
    size_t dvec_size_##T(dvec_##T dvec)\
    {\
        return dvec->size;\
    }\
\
    void dvec_append_##T(dvec_##T dvec, T value)\
    {\
        if (dvec->size == dvec->capacity)\
        {\
            dvec->capacity *= 2;\
            dvec->data = realloc(dvec->data, sizeof(T) * dvec->capacity);\
        }\
        dvec->data[dvec->size] = value;\
        dvec->size += 1;\
    }

DVEC_TYPE_DEFN(i8)
DVEC_TYPE_DEFN(i16)
DVEC_TYPE_DEFN(i32)
DVEC_TYPE_DEFN(i64)
DVEC_TYPE_DEFN(u8)
DVEC_TYPE_DEFN(u16)
DVEC_TYPE_DEFN(u32)
DVEC_TYPE_DEFN(u64)
DVEC_TYPE_DEFN(f32)
DVEC_TYPE_DEFN(f64)
DVEC_TYPE_DEFN(str)
DVEC_TYPE_DEFN(VkRect2D)