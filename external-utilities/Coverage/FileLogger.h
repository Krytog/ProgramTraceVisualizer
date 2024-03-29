#pragma once

#include <cstddef>
#include <string>
#include <fstream>
#include <mutex>

class FileLogger {
public:
    /* is_alive is used to prevent execution of coverage logging with invalid FileLogger, as there's no
     * gurantees of DSO statics destruction order */
    __attribute__((no_sanitize("coverage")))
    FileLogger(const std::string& output_file, size_t buffer_size, bool* is_alive);

    __attribute__((no_sanitize("coverage"))) ~FileLogger();

    __attribute__((no_sanitize("coverage"))) void Write(const std::string& data);

    __attribute__((no_sanitize("coverage"))) void Write(const char* data, size_t size);

private:
    __attribute__((no_sanitize("coverage"))) void DumpBuffer();

    std::string buffer_;
    size_t pos_to_write_;
    std::ofstream file_;
    std::mutex mutex_;
    bool* is_alive_;
};