#include "FileLogger.h"

#define COVERAGE_LOG_FILENAME "captured_ip.trace"
#define BUFFER_SIZE 1048576ull

static thread_local bool log_guard = false;
static bool log_alive = false;

static FileLogger* logger;

extern "C" void __sanitizer_cov_trace_pc_guard_init(uint32_t* start, uint32_t* stop) {  // NOLINT
    if (start == stop || *start || log_alive) {
        return;
    }
    static FileLogger logger_object(COVERAGE_LOG_FILENAME, BUFFER_SIZE, &log_alive);
    logger = &logger_object;
    log_alive = true;
}

extern "C" void __sanitizer_cov_trace_pc_guard(uint32_t* guard) {  // NOLINT
    if (log_guard || !log_alive) {
        return;
    }
    (void)guard;  // in this implementation guards are omitted
    void* pc = __builtin_return_address(0);
    log_guard =
        true;  // this is done to prevent recursively falling into this function from within logger->Write
    logger->Write((const char*)&pc, sizeof(void*));
    log_guard = false;
}