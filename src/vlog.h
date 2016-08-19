#ifndef __VLOG_H__
#define __VLOG_H__

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VLOG_ERR        0
#define VLOG_WARN       1
#define VLOG_INFO       2
#define VLOG_DEBUG      3

#ifdef __ANDROID__
#include <android/log.h>
#define LOG_TAG "wicore"
#endif

extern int log_level;

#if !defined(__ANDROID__)
#define vlogE(format, ...) \
    do { \
        if (log_level >= VLOG_ERR) \
            eclog(VLOG_ERR, format, ##__VA_ARGS__); \
    } while(0)
#else
#define vlogE(format, args...) \
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, format, ##args)
#endif

#if !defined(__ANDROID__)
#define vlogW(format, ...) \
    do { \
        if (log_level >= VLOG_WARN) \
            eclog(VLOG_WARN, format, ##__VA_ARGS__); \
    } while(0)
#else
#define vlogW(format, args...) \
    __android_log_print(ANDROID_LOG_WARN, LOG_TAG, format, ##args) 
#endif

#if !defined(__ANDROID__)
#define vlogI(format, ...) \
    do { \
        if (log_level >= VLOG_INFO) \
            eclog(VLOG_INFO, format, ##__VA_ARGS__); \
    } while(0)
#else
#define vlogI(format, args...) \
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, format, ##args) 
#endif

#if !defined(__ANDROID__)
#define vlogD(format, ...) \
    do { \
        if (log_level >= VLOG_DEBUG) \
            eclog(VLOG_DEBUG, format, ##__VA_ARGS__); \
    } while(0)
#else
#define vlogD(format, args...) \
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, format, ##args)
#endif


void eclog(int level, const char *format, ...);
void eclog_set_level(int level);

#ifdef __cplusplus
}
#endif

#endif
