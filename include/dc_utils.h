#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define TERM_BLACK          "\e[0;30m"
#define TERM_RED            "\e[0;31m"
#define TERM_GREEN          "\e[0;32m"
#define TERM_YELLOW         "\e[0;33m"
#define TERM_BLUE           "\e[0;34m"
#define TERM_PURPLE         "\e[0;35m"
#define TERM_CYAN           "\e[0;36m"
#define TERM_WHITE          "\e[0;37m"
#define TERM_RESET_COLOR    "\e[0m"

#define INFO_LOG_PREFIX         " INFO       "
#define WARN_LOG_PREFIX         " WARN       "
#define ERROR_LOG_PREFIX        " ERROR      "
#define SDL_ERROR_LOG_PREFIX    " SDL2 ERROR "
#define SUCCESS_LOG_PREFIX      " SUCCESS    "
#define VK_CHECK_PREFIX         " VK CHECK   "

#define TIMESTAMP_STR_MAX_LEN 20

enum TIME_UNIT
{
    NANOSECONDS = 0,
    MICROSECONDS,
    MILLISECONDS,
    SECONDS,
    MINUTES,
    HOURS
};

typedef float f32;
typedef double f64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef char* str;

#ifdef DEBUG
void _LOG(const char* prefix_color, const char* prefix, const char* file, i32 line, const char* func, const char* fmt, ...);
#define INFO_LOG(fmt, args...) _LOG(TERM_WHITE, INFO_LOG_PREFIX, __FILE__, __LINE__, __func__, fmt, ##args)
#define WARN_LOG(fmt, args...) _LOG(TERM_YELLOW, WARN_LOG_PREFIX, __FILE__, __LINE__, __func__, fmt, ##args)
#define ERR_LOG(fmt, args...) _LOG(TERM_RED, ERROR_LOG_PREFIX, __FILE__, __LINE__, __func__, fmt, ##args)
#define SDL_ERR_LOG(fmt, args...) _LOG(TERM_RED, SDL_ERROR_LOG_PREFIX, __FILE__, __LINE__, __func__, fmt, ##args)
#define SUCCESS_LOG(fmt, args...) _LOG(TERM_GREEN, SUCCESS_LOG_PREFIX, __FILE__, __LINE__, __func__, fmt, ##args)
void _VK_CHECK(const char* file, i32 line, const char* func, i64 vk_result, const char* fcall);
#define VK_CHECK(fcall) _VK_CHECK(__FILE__, __LINE__, __func__, fcall, #fcall)
#else
#define INFO_LOG(fmt, args...)
#define WARN_LOG(fmt, args...)
#define ERR_LOG(fmt, args...)
#define SDL_ERR_LOG(fmt, args...)
#define SUCCESS_LOG(fmt, args...)
#define VK_CHECK(fcall) fcall
#endif

char* format_string(const char* fmt, va_list args);
void get_timestamp(char timestamp[TIMESTAMP_STR_MAX_LEN]);
u64 get_counter(void);
u64 get_counter_frequency(void);
f64 get_delta(u64 start_time, enum TIME_UNIT tu);
f64 get_delta2(u64 start_time, u64 end_time, enum TIME_UNIT tu);
char* read_file(const char* path, size_t* out_size);

typedef struct str_vector
{
    char** data;
    size_t size;
} str_vector;

str_vector str_vector_init(void);
void str_vector_free(str_vector* v);
void str_vector_append(str_vector* v, char* str);
void str_vector_print(str_vector* v, const char* description);


typedef struct str_llist_node
{
    struct str_llist_node* next;
    char* value;
} str_llist_node;

typedef struct str_llist
{
    struct str_llist_node* head;
    struct str_llist_node* tail;
    size_t size;
} str_llist;

#endif