#pragma once

#include "../sys/sys_defines.h"
#include "../sys/sys_includes.h"
#include "../sys/sys_types.h"

enum class LogLevel : u8 { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };

class zLogger {
private:
    FILE*    m_stream;
    LogLevel m_min_level;

    void write(const char* file, int line, LogLevel level, const char* fmt,
               va_list args, const char* before = "", const char* after = "\n");
public:
    zLogger();                      // standard output
    zLogger(const char* filename);  // file
    zLogger(FILE* stream);          // external stream (e.g. stderr), not owned
    ~zLogger();

    void set_level(LogLevel level) { m_min_level = level; }

    void trace(const char* fmt, ...);
    void debug(const char* fmt, ...);
    void info(const char* fmt, ...);
    void warn(const char* fmt, ...);
    void error(const char* fmt, ...);
    void fatal(const char* fmt, ...);

    void trace(const char* file, int line, const char* fmt, ...);
    void debug(const char* file, int line, const char* fmt, ...);
    void info(const char* file, int line, const char* fmt, ...);
    void warn(const char* file, int line, const char* fmt, ...);
    void error(const char* file, int line, const char* fmt, ...);
    void fatal(const char* file, int line, const char* fmt, ...);
};

extern zLogger* g_defaultLogger;

#define LOG_TRACE(logger, ...) logger->trace(LOCATION, __VA_ARGS__)
#define LOG_DEBUG(logger, ...) logger->debug(LOCATION, __VA_ARGS__)
#define LOG_INFO(logger, ...)  logger->info(LOCATION, __VA_ARGS__)
#define LOG_WARN(logger, ...)  logger->warn(LOCATION, __VA_ARGS__)
#define LOG_ERROR(logger, ...) logger->error(LOCATION, __VA_ARGS__)
#define LOG_FATAL(logger, ...) logger->fatal(LOCATION, __VA_ARGS__)
