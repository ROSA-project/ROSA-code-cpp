#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"

#define LOG_DEBUG SPDLOG_DEBUG
#define LOG_TRACE SPDLOG_TRACE
#define LOG_INFO SPDLOG_INFO
#define LOG_WARN SPDLOG_WARN
#define LOG_ERROR SPDLOG_ERROR
#define LOG_CRITICAL SPDLOG_CRITICAL

[[maybe_unused]] static void set_logger() {
    // example: [17:24:31:809] [info] [6158] log message (run_simulator.cpp::main:14)
    spdlog::set_pattern("[%H:%M:%S:%e] %^[%l]%$ [%t] %v (%s::%!:%#)");

    //     auto console = spdlog::stdout_logger_mt("console");
    // spdlog::set_default_logger(console);
}
