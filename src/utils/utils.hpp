#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

void format_current_time(char* output);

#define UNUSED(x) ((void)(x))

#ifdef WIN32
#define MESH_EXPORT __declspec(dllexport)
#else
#define MESH_EXPORT \
    extern "C" __attribute__((visibility("default"))) __attribute__((used))
#endif  // WIN32

#ifdef MESH_DEBUG
#define MESH_DEBUG_FUNC __attribute__((visibility("hidden")))
#else
#define MESH_DEBUG_FUNC
#endif  // MESH_DEBUG

#define __mesh_log_skeleton(name, args...)      \
    do {                                        \
        char* time = malloc(sizeof(char) * 64); \
        assert(time != NULL);                   \
        memset(time, 0, sizeof(char) * 64);     \
        format_current_time(time);              \
        printf("%s " name " ", time);           \
        printf(args);                           \
        free(time);                             \
    } while (0)

#define mesh_log(args...) __mesh_log_skeleton("[LOG] ", args)
#define mesh_info(args...) __mesh_log_skeleton("[INFO]", args)
#define mesh_warn(args...) __mesh_log_skeleton("[WARN]", args)

#define mesh_debug(args...)                                        \
    do {                                                           \
        printf("%s %s:%d [DEBUG] ", __func__, __FILE__, __LINE__); \
        printf(args);                                              \
    } while (0)

#define mesh_todo(args...)                                         \
    do {                                                           \
        printf("%s %s:%d [DEBUG] ", __func__, __FILE__, __LINE__); \
        printf(args);                                              \
        fflush(stdout);                                            \
        abort();                                                   \
    } while (0)

// https://stackoverflow.com/a/25108449
#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i)                               \
    (((i) & 0x80ll) ? '1' : '0'), (((i) & 0x40ll) ? '1' : '0'),     \
        (((i) & 0x20ll) ? '1' : '0'), (((i) & 0x10ll) ? '1' : '0'), \
        (((i) & 0x08ll) ? '1' : '0'), (((i) & 0x04ll) ? '1' : '0'), \
        (((i) & 0x02ll) ? '1' : '0'), (((i) & 0x01ll) ? '1' : '0')

#define PRINTF_BINARY_PATTERN_INT16 \
    PRINTF_BINARY_PATTERN_INT8 PRINTF_BINARY_PATTERN_INT8
#define PRINTF_BYTE_TO_BINARY_INT16(i) \
    PRINTF_BYTE_TO_BINARY_INT8((i) >> 8), PRINTF_BYTE_TO_BINARY_INT8(i)
#define PRINTF_BINARY_PATTERN_INT32 \
    PRINTF_BINARY_PATTERN_INT16 PRINTF_BINARY_PATTERN_INT16
#define PRINTF_BYTE_TO_BINARY_INT32(i) \
    PRINTF_BYTE_TO_BINARY_INT16((i) >> 16), PRINTF_BYTE_TO_BINARY_INT16(i)
#define PRINTF_BINARY_PATTERN_INT64 \
    PRINTF_BINARY_PATTERN_INT32 PRINTF_BINARY_PATTERN_INT32
#define PRINTF_BYTE_TO_BINARY_INT64(i) \
    PRINTF_BYTE_TO_BINARY_INT32((i) >> 32), PRINTF_BYTE_TO_BINARY_INT32(i)

template <typename T>
static inline T random_n(T min, T max) {
    double scale = rand() / static_cast<double>(RAND_MAX);
    return (static_cast<double>(min)) + scale * static_cast<double>(max - min);
}

static inline std::string random_string(size_t len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (size_t i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}