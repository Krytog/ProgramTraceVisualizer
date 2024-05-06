#include <cstdio>
#include <cstring>
#include <cstddef>
#include <string>
#include <fstream>
#include <mutex>
#include <ctime>

class FileLogger {
public:
    /* is_alive is used to prevent execution of coverage logging with invalid FileLogger, as there's no
     * gurantees of DSO statics destruction order */
    __attribute__((no_sanitize("coverage")))
    FileLogger(const std::string& output_file, size_t buffer_size, bool* is_alive): pos_to_write_(0), file_(output_file), is_alive_(is_alive) {
        buffer_.resize(buffer_size);
    }

    __attribute__((no_sanitize("coverage"))) ~FileLogger() {
        *is_alive_ = false;
        DumpBuffer();
        file_.flush();
    }

    __attribute__((no_sanitize("coverage"))) void Write(const std::string& data) {
        Write(data.c_str(), data.size());
    }

    __attribute__((no_sanitize("coverage"))) void Write(const char* data, size_t size) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (pos_to_write_ + size >= buffer_.size()) {
            DumpBuffer();
        }
        std::memcpy(static_cast<void*>(&buffer_[0] + pos_to_write_), static_cast<const void*>(data), size);
        pos_to_write_ += size;
    }

private:
    __attribute__((no_sanitize("coverage"))) void DumpBuffer() {
        file_.write(buffer_.c_str(), pos_to_write_);
        pos_to_write_ = 0;
    }

    std::string buffer_;
    size_t pos_to_write_;
    std::ofstream file_;
    std::mutex mutex_;
    bool* is_alive_;
};

static std::string GetFileName() {
    time_t curr_time = time(nullptr);
	const auto* local_time = localtime(&curr_time);
    std::string output;
    output += std::to_string(local_time->tm_year + 1900) + "_";
    output += std::to_string(local_time->tm_mon + 1) + "_";
    output += std::to_string(local_time->tm_mday) + "_";
    output += std::to_string(local_time->tm_hour) + "_";
    output += std::to_string(local_time->tm_min) + "_";
    output += std::to_string(local_time->tm_sec) + ".trace";
	return output;
}

#define COVERAGE_LOG_FILENAME "captured_ip.trace"
#define BUFFER_SIZE 134217728ull

static thread_local bool log_guard = false;
static bool log_alive = false;

static FileLogger* logger;


extern "C" void __sanitizer_cov_trace_pc_guard_init(uint32_t* start, uint32_t* stop) {  // NOLINT
    if (start == stop || *start || log_alive) {
        return;
    }
    printf("we're cool %ld\n", stop - start);
    static FileLogger logger_object(GetFileName(), BUFFER_SIZE, &log_alive);
    logger = &logger_object;
    log_alive = true;
}

extern "C" void __sanitizer_cov_trace_pc_guard(uint32_t* guard) {  // NOLINT
    if (log_guard || !log_alive) {
        return;
    }
    (void)guard;  // in this implementation guards are omitted
    void* pc = __builtin_return_address(0);
    log_guard = true;  // this is done to prevent recursive calls of this function from within logger->Write
    logger->Write((const char*)&pc, sizeof(void*));
    log_guard = false;
}