#include "zodlib/container/zlogger.h"
#include "zodlib/sys/sys_print.h"

int main() {
    // --- sys_print / sys_error: free functions, no level, no timestamp ---
    sys_std_print("sys_print: plain message %d", 1);
    sys_error("net", "sys_error: tagged message %d", 2);

    // --- zLogger: stateful, leveled, timestamped ---
    zLogger console;
    console.trace("trace msg %d", 1);
    console.debug("debug msg %d", 2);
    console.info("info msg %d", 3);
    console.warn("warn msg %d", 4);
    console.error("error msg %d", 5);
    console.fatal("fatal msg %d", 6);

    console.set_level(LogLevel::WARN);
    console.info("this is filtered out, min level is WARN");
    console.warn("this still prints");

    zLogger file_log("zlogger_demo.log");
    file_log.info("written to zlogger_demo.log");

    // LOG_* macros: same as zLogger calls above, but also capture file:line
    // (debug builds only)
    LOG_ERROR(g_defaultLogger, "via defaultLogger: %s", "hello");

    return 0;
}
