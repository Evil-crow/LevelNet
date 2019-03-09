/**
 * Created by Crow on 3/2/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This class is the core of this library.
 *        Double-Cache to decrease the race condition.
 */

#ifndef LOGGER_ASYNC_LOGGING_H
#define LOGGER_ASYNC_LOGGING_H

#include <mutex>
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <condition_variable>

#include "log_buffer.h"
#include "log_file.h"

namespace log {

class async_logging {
 public:
  explicit async_logging(const std::string &file_name);
  async_logging(const std::string &file_name, size_t time);
  ~async_logging();
  void start();
  void stop();
  void append(const char *log_message, size_t len);
  void append_to_file();

 private:
  std::mutex mtx_;
  bool running_;
  std::condition_variable cond_;
  std::unique_ptr<log_buffer> front_first_buffer_;
  std::unique_ptr<log_buffer> front_second_buffer_;
  std::vector<std::unique_ptr<log_buffer>> buffers_;
  std::unique_ptr<std::thread> thread_;

  std::unique_ptr<log_file> file_;
  std::string file_name_;
  std::chrono::milliseconds time_;
};

}

#endif //LOGGER_ASYNC_LOGGING_H
