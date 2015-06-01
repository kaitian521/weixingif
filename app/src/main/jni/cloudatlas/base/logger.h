// Author: Yifan fan (yifan.fan@dian.fm)

#ifndef CLOUDATLAS_BASE_LOGGER_H_
#define CLOUDATLAS_BASE_LOGGER_H_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <string>
#include <vector>

#include "base/macros.h"
#include "cloudatlas/base/blocking_queue.h"

namespace cloudatlas {

class Logger {
 public:
  enum LogLevel {
    kTrace,
    kDebug,
    kInfo,
    kWarn,
    kError,
    kFatal,
    kNumLogLevels,
  };

  static Logger& GetInstance();
  ~Logger();

  static LogLevel log_level();
  static void set_log_level(const LogLevel level);

  std::string log_file_path() {
    return log_file_path_;
  }

  bool set_log_file_path(const std::string& path);

  bool log_cached() {
    return log_cached_;
  }

  void set_log_cached(const bool cached) {
    log_cached_ = cached;
  }

  void Log(const int priority,
           const char* caption,
           const bool need_sync,
           const char* file_name,
           const char* func_name,
           const int line_number,
           const char* format,
           ...);

  void ForceSync();

 private:
  Logger();

  void Fsync(const bool need_flush);

  // TODO(yifan.fan): ouput func can be set, so we can write log to disk or
  // remote server.
  // void OutputFunc(const char* msg, int len);

 private:
  static std::unique_ptr<Logger> instance_;
  static std::once_flag once_flag_;

  int log_fd_;
  bool log_cached_;
  std::string log_file_path_;

  std::mutex log_mutex_;
  std::vector<char> log_buf_;
  char *current_buf_pos_;

  DISABLE_COPY_AND_ASSIGN(Logger);
};

std::string strerror_tl(const int saved_errno);

extern Logger::LogLevel g_log_level;

inline Logger::LogLevel Logger::log_level() {
  return g_log_level;
}

// CHECK dies with a fatal error if condition is not true.  It is *not*
// controlled by NDEBUG, so the check will be executed regardless of
// compilation mode.
#define CHECK(condition, message)  \
  ((condition) ? ((void) 0) : LogFatal("Check failed: %s, %s", #condition, #message))

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.
#define CHECK_NOTNULL(val) \
  CheckNotNull(__FILE__, __func__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(const char* file, const char* func, int line, const char* names, T* ptr) {
  if (ptr == NULL) {
    cloudatlas::Logger::GetInstance().Log(cloudatlas::Logger::kFatal,
                                          "FATAL",
                                          true,
                                          file,
                                          func,
                                          line,
                                          "%s",
                                          names);
  }
  return ptr;
}

#ifdef NDEBUG
#define LOG_ASSERT(condition, message) ((void) 0)
#else
#define LOG_ASSERT(condition, message) \
  ((condition) ? ((void) 0) : LogFatal("Assert failed: %s, %s", #condition, #message))
#endif

// You can use these macros as C-Style printf. Note that
// '\n' will be automatically append to log msg.
#define LogFatal(format, ...) \
  cloudatlas::Logger::GetInstance().Log(cloudatlas::Logger::kFatal, \
                                        "FATAL", \
                                        true, \
                                        __FILE__, \
                                        __func__, \
                                        __LINE__, \
                                        format, \
                                        ##__VA_ARGS__)

#define LogError(format, ...) \
  cloudatlas::Logger::GetInstance().Log(cloudatlas::Logger::kError, \
                                        "ERROR", \
                                        true, \
                                        __FILE__, \
                                        __func__, \
                                        __LINE__, \
                                        format, \
                                        ##__VA_ARGS__)

#define LogWarn(format, ...) \
  cloudatlas::Logger::GetInstance().Log(cloudatlas::Logger::kWarn, \
                                        "WARN", \
                                        true, \
                                        __FILE__, \
                                        __func__, \
                                        __LINE__, \
                                        format, \
                                        ##__VA_ARGS__)

#define LogInfo(format, ...) \
  if (cloudatlas::Logger::kInfo >= cloudatlas::Logger::log_level()) \
    cloudatlas::Logger::GetInstance().Log(cloudatlas::Logger::kInfo, \
                                          "INFO", \
                                          false, \
                                          __FILE__, \
                                          __func__, \
                                          __LINE__, \
                                          format, \
                                          ##__VA_ARGS__)

#define LogDebug(format, ...) \
  if (cloudatlas::Logger::kDebug >= cloudatlas::Logger::log_level()) \
    cloudatlas::Logger::GetInstance().Log(cloudatlas::Logger::kDebug, \
                                          "DEBUG", \
                                          true, \
                                          __FILE__, \
                                          __func__, \
                                          __LINE__, \
                                          format, \
                                          ##__VA_ARGS__)

#define LogTrace(format, ...) \
  if (cloudatlas::Logger::kTrace >= cloudatlas::Logger::log_level()) \
    cloudatlas::Logger::GetInstance().Log(cloudatlas::Logger::kTrace, \
                                          "TRACE", \
                                          true, \
                                          __FILE__, \
                                          __func__, \
                                          __LINE__, \
                                          format, \
                                          ##__VA_ARGS__)

}  // namespace cloudatlas

#endif  // CLOUDATLAS_BASE_LOGGER_H_
