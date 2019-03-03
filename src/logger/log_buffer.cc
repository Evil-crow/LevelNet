/**
 * Created by Crow on 2/1/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "log_buffer.h"

#include <cstring>
#include <memory>
#include <iostream>

namespace log {

log_buffer::log_buffer(size_t size)
    : index_(0),
      size_(size > max_data_size ? max_data_size : size),
      data_(std::make_unique<char []>(size_))
{
  ;
}

log_buffer::log_buffer() : log_buffer(max_data_size) { ; }

bool log_buffer::append(const char *str, size_t len)
{
  if (available() > len) {
    ::memcpy(data_.get() + index_, str, len);
    index_ += len;
    return true;
  }

  return false;
}

size_t log_buffer::available() const
{
  return size_ - index_;
}

size_t log_buffer::size() const
{
  return size_;
}

size_t log_buffer::count() const
{
  return index_;
}

char *log_buffer::data() const
{
  return data_.get();
}

void log_buffer::reset()
{
  ::memset(data_.get(), 0, sizeof(size_));
  index_ = 0;
}

}
