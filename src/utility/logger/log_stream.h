/**
 * Created by Crow on 2/1/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This class is like C++ Standard stream series, but write to log_buffer,
 *         may be like <fstream>
 */

#ifndef LOGGER_LOG_STREAM_H
#define LOGGER_LOG_STREAM_H

#include "log_buffer.h"
#include <cstring>
#include <functional>

namespace log {

class log_stream {
 public:
  ~log_stream();
  static void output_message_func(std::function<void(const char *, size_t)> functor);

  log_stream &operator<<(short val);
  log_stream &operator<<(unsigned short val);
  log_stream &operator<<(int val);
  log_stream &operator<<(unsigned int val);
  log_stream &operator<<(long val);
  log_stream &operator<<(unsigned long val);
  log_stream &operator<<(long long val);
  log_stream &operator<<(unsigned long long val);
  log_stream &operator<<(float val);
  log_stream &operator<<(double val);

  log_stream &operator<<(char val) {
    buffer_.append(&val, 1);
    return *this;
  }

  log_stream &operator<<(bool val) {
    buffer_.append(val ? "T" : "F", 1);
    return *this;
  }

  log_stream &operator<<(const char *str) {
    if (str) {
      buffer_.append(str, std::strlen(str));
    } else {
      buffer_.append("(null)", 6);
    }

    return *this;
  }

  log_stream &operator<<(const unsigned char *str) {
    return operator<<(reinterpret_cast<const char *>(str));
  }

  log_stream &operator<<(const std::string &str) {
    buffer_.append(str.c_str(), str.size());
    return *this;
  }

  log_stream &operator<<(const log_buffer &buf) {
    buffer_.append(buffer_.data(), buffer_.count());
    return *this;
  }

  void reset_buffer() {
    buffer_.reset();
  }
  static std::function<void(const char *, size_t)> output_func_;

 private:
  log_buffer buffer_;
};

}

#endif //LOGGER_LOG_STREAM_H
