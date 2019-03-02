/**
 * Created by Crow on 2/2/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This class log_file is used to write the message to disk
 *      It can set the file and buffer to write.
 *      It used C++ FileStream to write for portability
 */

#ifndef LOGGER_LOG_FILE_H
#define LOGGER_LOG_FILE_H

#include <string>
#include <memory>
#include <fstream>

namespace log {

class log_file {
 public:
  explicit log_file(const std::string &file_name);
  void flush(const char *str);

 private:
  std::unique_ptr<std::fstream> stream_;
};

}

#endif //LOGGER_LOG_FILE_H
