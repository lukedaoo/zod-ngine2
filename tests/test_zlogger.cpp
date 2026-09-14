#include <UnitTest++/UnitTest++.h>

#include "zodlib/precompiled.h"

namespace {
usize read_file(const char* path, char* out, usize out_size) {
    FILE* f = std::fopen(path, "r");
    if (!f) {
        return 0;
    }
    usize n = std::fread(out, 1, out_size - 1, f);
    out[n]  = '\0';
    std::fclose(f);
    return n;
}
}  // namespace

SUITE(zlogger) {
    TEST(test_zlogger_writes_to_file) {
        const char* path = "test_zlogger_writes.log";
        {
            zLogger log(path);
            log.info("hello %d", 42);
        }

        char buf[256];
        read_file(path, buf, sizeof(buf));
        CHECK(std::strstr(buf, "[INFO] hello 42") != nullptr);

        std::remove(path);
    }

    TEST(test_zlogger_level_filter) {
        const char* path = "test_zlogger_filter.log";
        {
            zLogger log(path);
            log.set_level(LogLevel::WARN);
            log.info("filtered out");
            log.warn("kept");
        }

        char buf[256];
        read_file(path, buf, sizeof(buf));
        CHECK(std::strstr(buf, "filtered out") == nullptr);
        CHECK(std::strstr(buf, "kept") != nullptr);

        std::remove(path);
    }

    TEST(test_zlogger_truncates_long_message) {
        const char* path = "test_zlogger_truncate.log";
        {
            zLogger log(path);
            char    long_msg[MAX_PRINT_MESSAGE_CHARS + 1000];
            std::memset(long_msg, 'a', sizeof(long_msg) - 1);
            long_msg[sizeof(long_msg) - 1] = '\0';
            log.info("%s", long_msg);
        }

        FILE* f = std::fopen(path, "r");
        CHECK(f != nullptr);
        std::fseek(f, 0, SEEK_END);
        long size = std::ftell(f);
        std::fclose(f);

        CHECK(size < MAX_PRINT_MESSAGE_CHARS + 100);

        std::remove(path);
    }

    TEST(test_zlogger_bad_path_falls_back_without_crash) {
        zLogger log("/nonexistent_dir/whatever.log");
        log.info("should not crash");
        CHECK(true);
    }

    TEST(test_zlogger_stderr_stream_not_closed_by_destructor) {
        {
            zLogger log(stderr);
            log.info("to stderr");
        }
        // stderr must still be usable after the logger destructs
        int ok = std::fputc('\n', stderr);
        CHECK(ok >= 0);
    }
}
