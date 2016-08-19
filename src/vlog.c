#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#ifdef __ANDROID__
#include <android/log.h>
#endif
#include "vlog.h"

#ifdef __ANDROID__
#define MAX_BUF_SZ ((int)1024)
#endif

#define TIME_FORMAT     "%Y-%m-%d %H:%M:%S"

int log_level = VLOG_INFO;

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static char* level_names[] = {
    "ERROR",
    "WARNING",
    "INFO",
    "DEBUG"
};

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>

static HANDLE hConsole = NULL;

static void set_color(int level)
{
    WORD attr;

    if (!hConsole)
        hConsole = GetStdHandle(STD_ERROR_HANDLE);

    if (level == VLOG_ERR)
        attr = FOREGROUND_RED;
    else if (level == VLOG_WARN)
        attr = FOREGROUND_RED | FOREGROUND_GREEN;
    else if (level == VLOG_INFO)
        attr = FOREGROUND_GREEN;
    else if (level == VLOG_DEBUG)
        attr = FOREGROUND_INTENSITY;

    SetConsoleTextAttribute(hConsole, attr);
}

static void reset_color()
{
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE
                            | FOREGROUND_GREEN | FOREGROUND_RED);
}
#else
static void set_color(int level)
{
    char *color = "";

    if (level == VLOG_ERR)
        color = "\e[0;31m";
    else if (level == VLOG_WARN)
        color = "\e[0;33m";
    else if (level == VLOG_INFO)
        color = "\e[0;32m";
    else if (level == VLOG_DEBUG)
        color = "\e[1;30m";

    fprintf(stderr, "%s", color);
}

static void reset_color()
{
    fprintf(stderr, "\e[0m");
}
#endif

void eclogv(int level, const char *format, va_list args)
{
    if (level > log_level)
        return;
    
    if (level > VLOG_DEBUG)
        level = VLOG_DEBUG;

#if defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    char timestr[20];
    time_t cur = time(NULL);
    strftime(timestr, 20, TIME_FORMAT, localtime(&cur));
    printf("%s [%s] ", timestr, level_names[level]);
    vprintf(format, args);
    printf("\n");
#elif defined(__ANDROID__)
    //to nothing;
#else
    {
        int rc;
        char timestr[20];
        char buf[512];
        char out[512];
        time_t cur = time(NULL);

        strftime(timestr, 20, TIME_FORMAT, localtime(&cur));
        rc = vsnprintf(buf, sizeof(buf), format, args);
        if (rc == -1)
            buf[511] = 0;

        rc = snprintf(out, sizeof(out), "%s - %-7s : %s\n",
                       timestr, level_names[level], buf);
        if (rc == -1)
            out[511] = 0;

        pthread_mutex_lock(&lock);
        set_color(level);
        fputs(out, stderr);
        reset_color();
        pthread_mutex_unlock(&lock);
        fflush(stderr);
    }
#endif
}

void eclog(int level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    eclogv(level, format, args);
    va_end(args);
}

void eclog_set_level(int level)
{
    if (level > VLOG_DEBUG)
        level = VLOG_DEBUG;

    log_level = level;
}

