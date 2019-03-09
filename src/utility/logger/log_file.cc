/**
 * Created by Crow on 2/2/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "log_file.h"
#include <fstream>
#include <fcntl.h>
#include <unistd.h>

namespace log {

log_file::log_file(const std::string &file_name)
    : fd_(::open(file_name.c_str(), O_RDWR | O_APPEND | O_CREAT, 0644))
{ ; }

log_file::~log_file()
{
  if (::close(fd_))
    perror("close");
}

void log_file::flush(const char *str, size_t count)
{
  if (::write(fd_, str, count) < 0) {
    perror("write");
    std::abort();
  }
}

}
