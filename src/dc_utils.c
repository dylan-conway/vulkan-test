#include "dc_utils.h"

#include <stdarg.h>
#include <time.h>
#include <SDL2/SDL.h>

#ifdef DEBUG
void _LOG(const char* prefix_color, const char* prefix, const char* file, i32 line, const char* func, const char* fmt, ...)
{
    char timestamp[TIMESTAMP_STR_MAX_LEN] = { '\0' };
    get_timestamp(timestamp);

    va_list args;
    va_start(args, fmt);
    char* str = format_string(fmt, args);
    va_end(args);

    char* saveptr = NULL;
    char* token = strtok_r(str, "\n", &saveptr);
    while (NULL != token)
    {
        fprintf(stdout, "%s %s%s" TERM_RESET_COLOR "[%s::%s:%d] >>>  %s\n", timestamp, prefix_color, prefix, file, func, line, token);
        token = strtok_r(NULL, "\n", &saveptr);
    }

    free(str);
}

void _VK_CHECK(const char* file, i32 line, const char* func, i64 vk_result, const char* fcall)
{
    char timestamp[TIMESTAMP_STR_MAX_LEN] = { '\0' };
    get_timestamp(timestamp);
    fprintf(stdout, "%s ", timestamp);
    if (vk_result == 0) fprintf(stdout, TERM_GREEN);
    else if (vk_result < 0) fprintf(stdout, TERM_RED);
    else fprintf(stdout, TERM_YELLOW);
    // TODO ("get string value of Vulkan enum")
    fprintf(stdout, VK_CHECK_PREFIX TERM_RESET_COLOR "[%s::%s:%d] >>>  %s\n", file, func, line, fcall);
}
#endif

char* format_string(const char* fmt, va_list args)
{
    i32 str_len = vsnprintf(NULL, 0, fmt, args);
    char* str = malloc(sizeof(char) * (str_len + 1));
    vsnprintf(str, str_len + 1, fmt, args);
    str[str_len] = '\0';
    return str;
}

void get_timestamp(char timestamp[TIMESTAMP_STR_MAX_LEN])
{
    struct timespec ts = { 0 };
    struct tm t = { 0 };
    clock_gettime(CLOCK_REALTIME, &ts);
    gmtime_s(&t, &ts.tv_sec);
    strftime(timestamp, TIMESTAMP_STR_MAX_LEN, "%H:%M:%S", &t);
    timestamp[8] = '.';
    snprintf(timestamp + 9, 10, "%09ld", ts.tv_nsec);
    timestamp[18] = 'Z';
    timestamp[19] = '\0';
}

u64 get_counter(void)
{
    return SDL_GetPerformanceCounter();
}

u64 get_counter_frequency(void)
{
    return SDL_GetPerformanceFrequency();
}

f64 get_delta(u64 start_time, enum TIME_UNIT tu)
{
    return get_delta2(start_time, get_counter(), tu);
}

f64 get_delta2(u64 start_time, u64 end_time, enum TIME_UNIT tu)
{
    if (start_time > end_time)
    {
        WARN_LOG("start time is greater than end time, cannot calculate delta");
        return -1.0f;
    }

    u64 freq = get_counter_frequency();
    u64 elapsed = end_time - start_time;

    switch (tu)
    {
        case MICROSECONDS: return (elapsed * 1000000.0f) / (f64) freq;
        case MILLISECONDS: return (elapsed * 1000.0f) / (f64) freq;
        case SECONDS: return (elapsed * 1.0f) / (f64) freq;
        default:
            ERR_LOG("unsupported time unit, defaulting to milliseconds...\n");
            return (elapsed * 1000.0f) / (f64) freq;
    }
}

char* read_file(const char* path, size_t* out_size)
{
    FILE* file = fopen(path, "rb");
    if (NULL == file)
    {
        ERR_LOG("failed to open file \"%s\"", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    char* contents = malloc(sizeof(char) * (size + 1));
    fread(contents, sizeof(char), size, file);
    contents[size] = '\0';
    fclose(file);
    *out_size = size;
    return contents;
}
