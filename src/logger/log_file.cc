/**
 * Created by Crow on 2/2/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "log_file.h"
#include <fstream>

namespace log {

log_file::log_file(const std::string &file_name)
    : stream_(std::make_unique<std::fstream>(file_name, std::ios::app))
{ ; }

void log_file::flush(const char *str)
{
  stream_->operator<<(str);
}

}
