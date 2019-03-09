#ifndef LOGGER_LIBRARY_H
#define LOGGER_LIBRARY_H

#include <string>
#include "async_logging.h"
#include "log_stream.h"

namespace log {

#define WARN "warn"
#define INFO "info"
#define ERROR "error"
#define LOG(PRIORITY) log::logger::get_log().set_level(PRIORITY); log::logger::get_log()

class logger {
 public:
  ~logger() = default;

  static logger &get_log() {
    static logger instance("/home/Crow/logxxx.log");
    return instance;
  }
  void set_level(const char *level)   { level_ = level; }

  template <typename T>
  logger &operator<<(T message)
  {
    log_stream::output_message_func(
        [this](const char *log, size_t len){
          log_->append(log, len);
        });

    log_stream().operator<<(message);
  }

//  template<short> inline logger &operator<<(short message);
//  template logger &operator<< <unsigned short>(unsigned short message);
//  template logger &operator<< <int>(int message);
//  template logger &operator<< <unsigned int>(unsigned int message);
//  template logger &operator<< <long>(long message);
//  template logger &operator<< <unsigned long>(unsigned long message);
//  template logger &operator<< <long long>(long long message);
//  template logger &operator<< <unsigned long long>(unsigned long long message);
//  template logger &operator<< <float>(float message);
//  template logger &operator<< <double>(double message);
//  template logger &operator<< <char>(char message);
//  template logger &operator<< <bool>(bool message);
//  template logger &operator<< <const char *>(const char *message);
//  template logger &operator<< <const unsigned char *>(const unsigned char *message);
//  template logger &operator<< <const std::string &>(const std::string &message);

 private:
  logger(const std::string &file_name, int seconds)
      : level_("info"), log_(std::make_unique<async_logging>(file_name, seconds)) {
    log_->start();
  }

  explicit logger(const std::string &file_name)
      : level_("info"), log_(std::make_unique<async_logging>(file_name)) {
    log_->start();
  }

  std::string level_;
  std::unique_ptr<async_logging> log_;
};

}

#endif