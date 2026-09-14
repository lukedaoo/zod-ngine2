#include "zlogger.h"

#include <ctime>

#include "../sys/sys_print.h"

static const char* level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE:
            return "TRACE";
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
    }
    return "?";
}

zLogger::zLogger() : m_stream(stdout), m_min_level(LogLevel::TRACE) {}

zLogger::zLogger(const char* filename)
    : m_stream(std::fopen(filename, "a")), m_min_level(LogLevel::TRACE) {
    if (!m_stream) {
        sys_std_error("zLogger", "failed to open '%s', falling back to stdout",
                      filename);
        m_stream = stdout;
    }
}

zLogger::zLogger(FILE* stream)
    : m_stream(stream), m_min_level(LogLevel::TRACE) {}

zLogger::~zLogger() {
    if (m_stream != stdout && m_stream != stderr) {
        std::fclose(m_stream);
    }
}

void zLogger::write(const char* file, int line, LogLevel level, const char* fmt,
                    va_list args, const char* before, const char* after) {
    if (level < m_min_level) {
        return;
    }

    std::time_t now = std::time(nullptr);
    char        time_buf[16];
    std::strftime(time_buf, sizeof(time_buf), "%H:%M:%S", std::localtime(&now));

    std::fprintf(m_stream, "[%s] [%s] %s", time_buf, level_to_string(level),
                 before);
    sys_vfprint(m_stream, fmt, args);
#if defined(_DEBUG)
    if (file) {
        std::fprintf(m_stream, " %s:%d", file, line);
    }
#endif
    std::fputs(after, m_stream);
}

#define ZLOGGER_LEVEL_METHODS(_func_name_, _log_level_)                    \
    void zLogger::_func_name_(const char* fmt, ...) {                      \
        va_list args;                                                      \
        va_start(args, fmt);                                               \
        write(nullptr, 0, _log_level_, fmt, args);                         \
        va_end(args);                                                      \
    }                                                                      \
    void zLogger::_func_name_(const char* file, int line, const char* fmt, \
                              ...) {                                       \
        va_list args;                                                      \
        va_start(args, fmt);                                               \
        write(file, line, _log_level_, fmt, args);                         \
        va_end(args);                                                      \
    }

ZLOGGER_LEVEL_METHODS(trace, LogLevel::TRACE)
ZLOGGER_LEVEL_METHODS(debug, LogLevel::DEBUG)
ZLOGGER_LEVEL_METHODS(info, LogLevel::INFO)
ZLOGGER_LEVEL_METHODS(warn, LogLevel::WARN)
ZLOGGER_LEVEL_METHODS(error, LogLevel::ERROR)
ZLOGGER_LEVEL_METHODS(fatal, LogLevel::FATAL)

#undef ZLOGGER_LEVEL_METHODS

static zLogger defaultLoggerLocal = {};
zLogger*       g_defaultLogger    = &defaultLoggerLocal;
