#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace recoil {

class Logger {
public:
    static void Initialize(const std::string& log_file = "recoil_rcs.log");
    static void Shutdown();
    
    static std::shared_ptr<spdlog::logger> Get() { return logger_; }
    
private:
    static std::shared_ptr<spdlog::logger> logger_;
};

#define LOG_TRACE(...)    ::recoil::Logger::Get()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)    ::recoil::Logger::Get()->debug(__VA_ARGS__)
#define LOG_INFO(...)     ::recoil::Logger::Get()->info(__VA_ARGS__)
#define LOG_WARN(...)     ::recoil::Logger::Get()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    ::recoil::Logger::Get()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::recoil::Logger::Get()->critical(__VA_ARGS__)

} // namespace recoil
