/**
 * Created by Crow on 2/1/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This class will be used for Log Message Buffer
 */

#ifndef LOGGER_LOG_BUFFER_H
#define LOGGER_LOG_BUFFER_H

#include <bits/unique_ptr.h>
namespace log {

class log_buffer {
 public:
  log_buffer();
  explicit log_buffer(size_t size);

  bool append(const char *str, size_t len);
  size_t available() const;
  size_t size() const;
  size_t count() const;
  char * data() const;
  void reset();

 private:
  static const size_t max_data_size = 4 * 1000;
  size_t index_;
  size_t size_;
  std::unique_ptr<char []> data_;
};

}

#endif //LOGGER_LOG_BUFFER_H
