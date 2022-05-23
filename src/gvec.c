#include "gvec.h"
#include <inttypes.h>

#define GVEC_TYPE_DATA(t) t* t##_data
struct generic_vector
{
    enum gvec_type type;
    union
    {
        GVEC_TYPE_DATA(i8);
        GVEC_TYPE_DATA(i16);
        GVEC_TYPE_DATA(i32);
        GVEC_TYPE_DATA(i64);
        GVEC_TYPE_DATA(u8);
        GVEC_TYPE_DATA(u16);
        GVEC_TYPE_DATA(u32);
        GVEC_TYPE_DATA(u64);
        GVEC_TYPE_DATA(f32);
        GVEC_TYPE_DATA(f64);
        GVEC_TYPE_DATA(str);
        GVEC_TYPE_DATA(VkExtent2D);
        GVEC_TYPE_DATA(VkRect2D);
        GVEC_TYPE_DATA(VkViewport);
    };
    size_t size;
    u32 length;
};

gvec gvec_init(enum gvec_type type)
{
    gvec vec = calloc(1, sizeof(struct generic_vector));
    vec->type = type;
    return vec;
}

#define GVEC_FREE_CASE(t) \
case gvec_##t: \
    free(vec->t##_data); \
    break \

void gvec_free(gvec vec)
{
    switch(vec->type)
    {
        GVEC_FREE_CASE(i8);
        GVEC_FREE_CASE(i16);
        GVEC_FREE_CASE(i32);
        GVEC_FREE_CASE(i64);
        GVEC_FREE_CASE(u8);
        GVEC_FREE_CASE(u16);
        GVEC_FREE_CASE(u32);
        GVEC_FREE_CASE(u64);
        GVEC_FREE_CASE(f32);
        GVEC_FREE_CASE(f64);
        GVEC_FREE_CASE(str);
        GVEC_FREE_CASE(VkExtent2D);
        GVEC_FREE_CASE(VkRect2D);
        GVEC_FREE_CASE(VkViewport);
        case gvec_unassigned:
        {
            WARN_LOG("attempted freeing unassigned gvec");
            break;
        }
    }
    vec->type = gvec_unassigned;
    vec->size = 0;
    free(vec);
}

void gvec_print(gvec vec, const char* description)
{
    switch (vec->type)
    {
        case gvec_i8:
        {
            INFO_LOG("%s <gvec_i8>:", description);
            if (0 == vec->size)
            {
                INFO_LOG("\tempty vector");
            }
            else
            {
                i8* data = NULL;
                gvec_data(vec, &data);
                for (u64 i = 0; i < vec->size; i ++)
                {
                    INFO_LOG("\t%d: %" PRId8, i, data[i]);
                }
            }
            break;
        }
        case gvec_i16:
        {
            INFO_LOG("%s <gvec_i16>:", description);
            if (0 == vec->size)
            {
                INFO_LOG("\tempty vector");
            }
            else
            {
                i16* data = NULL;
                gvec_data(vec, &data);
                for (u64 i = 0; i < vec->size; i ++)
                {
                    INFO_LOG("\t%d: %" PRId16, i, data[i]);
                }
            }
            break;
        }
        case gvec_i32:
        {
            INFO_LOG("%s <gvec_i32>:", description);
            if (0 == vec->size)
            {
                INFO_LOG("\tempty vector");
            }
            else
            {
                i32* data = NULL;
                gvec_data(vec, &data);
                for (u64 i = 0; i < vec->size; i ++)
                {
                    INFO_LOG("\t%d: %" PRId32, i, data[i]);
                }
            }
            break;
        }
        case gvec_i64:
        {
            INFO_LOG("%s <gvec_i64>:", description);
            if (0 == vec->size)
            {
                INFO_LOG("\tempty vector");
            }
            else
            {
                i64* data = NULL;
                gvec_data(vec, &data);
                for (u64 i = 0; i < vec->size; i ++)
                {
                    INFO_LOG("\t%d: %" PRId64, i, data[i]);
                }
            }
            break;
        }
        case gvec_u8:
        {
            INFO_LOG("%s <gvec_u8>:", description);
            if (0 == vec->size)
            {
                INFO_LOG("\tempty vector");
            }
            else
            {
                u8* data = NULL;
                gvec_data(vec, &data);
                for (u64 i = 0; i < vec->size; i ++)
                {
                    INFO_LOG("\t%d: %" PRIu8, i, data[i]);
                }
            }
            break;
        }
        case gvec_u16:
        {
            INFO_LOG("%s <gvec_u16>:", description);
            if (0 == vec->size)
            {
                INFO_LOG("\tempty vector");
            }
            else
            {
                u16* data = NULL;
                gvec_data(vec, &data);
                for (u64 i = 0; i < vec->size; i ++)
                {
                    INFO_LOG("\t%d: %" PRIu16, i, data[i]);
                }
            }
            break;
        }
        case gvec_u32:
        {
            INFO_LOG("%s <gvec_u32>:", description);
            if (0 == vec->size)
            {
                INFO_LOG("\tempty vector");
            }
            else
            {
                u32* data = NULL;
                gvec_data(vec, &data);
                for (u64 i = 0; i < vec->size; i ++)
                {
                    INFO_LOG("\t%d: %" PRIu32, i, data[i]);
                }
            }
            break;
        }
        case gvec_u64:
        {
            INFO_LOG("%s <gvec_u64>:", description);
            if (0 == vec->size)
            {
                INFO_LOG("\tempty vector");
            }
            else
            {
                u64* data = NULL;
                gvec_data(vec, &data);
                for (u64 i = 0; i < vec->size; i ++)
                {
                    INFO_LOG("\t%d: %" PRIu64, i, data[i]);
                }
            }
            break;
        }
        case gvec_f32:
        {
            INFO_LOG("%s <gvec_f32>:", description);
            if (0 == vec->size)
            {
                INFO_LOG("\tempty vector");
            }
            else
            {
                f32* data = NULL;
                gvec_data(vec, &data);
                for (u64 i = 0; i < vec->size; i ++)
                {
                    INFO_LOG("\t%d: %f", i, data[i]);
                }
            }
            break;
        }
        case gvec_f64:
        {
            INFO_LOG("%s <gvec_f64>:", description);
            if (0 == vec->size)
            {
                INFO_LOG("\tempty vector");
            }
            else
            {
                f64* data = NULL;
                gvec_data(vec, &data);
                for (u64 i = 0; i < vec->size; i ++)
                {
                    INFO_LOG("\t%d: %f", i, data[i]);
                }
            }
            break;
        }
        case gvec_str:
        {
            INFO_LOG("%s <gvec_str>:", description);
            if (0 == vec->size)
            {
                INFO_LOG("\tempty vector");
            }
            else
            {
                char** data = NULL;
                gvec_data(vec, &data);
                for (u64 i = 0; i < vec->size; i ++)
                {
                    INFO_LOG("\t%d: %s", i, data[i]);
                }
            }
            break;
        }
        case gvec_unassigned:
        {
            INFO_LOG("%s <gvec_unassigned>:", description);
            INFO_LOG("\tunknown data, should be empty");
            break;
        }
        default:
        {
            WARN_LOG("PRINT NOT IMPLEMENTED");
        }
    }
}

enum gvec_type gvec_type(gvec vec)
{
    return vec->type;
}

size_t gvec_size(gvec vec)
{
    return vec->size;
}

void gvec_append_error(void)
{
    ERR_LOG("failed to append value to gvec, unimplemented type");
}

void gvec_data_error(void)
{
    ERR_LOG("failed to get data from gvec, unimplemented type");
}

#define GVEC_APPEND_DEFN(t) \
void gvec_append_##t(gvec vec, t value) \
{ \
    if (gvec_##t != vec->type) \
    { \
        ERR_LOG("incorrect gvec append type"); \
    } \
    vec->t##_data = realloc(vec->t##_data, sizeof(t) * (vec->size + 1)); \
    vec->t##_data[vec->size] = value; \
    vec->size += 1; \
}

GVEC_APPEND_DEFN(i8)
GVEC_APPEND_DEFN(i16)
GVEC_APPEND_DEFN(i32)
GVEC_APPEND_DEFN(i64)
GVEC_APPEND_DEFN(u8)
GVEC_APPEND_DEFN(u16)
GVEC_APPEND_DEFN(u32)
GVEC_APPEND_DEFN(u64)
GVEC_APPEND_DEFN(f32)
GVEC_APPEND_DEFN(f64)
GVEC_APPEND_DEFN(str)
GVEC_APPEND_DEFN(VkExtent2D)
GVEC_APPEND_DEFN(VkRect2D);
GVEC_APPEND_DEFN(VkViewport);

#define GVEC_DATA_DEFN(t) \
void gvec_data_##t(gvec vec, t** data_out) \
{ \
    if (gvec_##t != vec->type) \
    { \
        ERR_LOG("incorrect gvec get data type"); \
        return; \
    } \
    if (NULL != data_out) *data_out = vec->t##_data; \
}

GVEC_DATA_DEFN(i8)
GVEC_DATA_DEFN(i16)
GVEC_DATA_DEFN(i32)
GVEC_DATA_DEFN(i64)
GVEC_DATA_DEFN(u8)
GVEC_DATA_DEFN(u16)
GVEC_DATA_DEFN(u32)
GVEC_DATA_DEFN(u64)
GVEC_DATA_DEFN(f32)
GVEC_DATA_DEFN(f64)
GVEC_DATA_DEFN(str)
GVEC_DATA_DEFN(VkExtent2D)
GVEC_DATA_DEFN(VkRect2D);
GVEC_DATA_DEFN(VkViewport);
