/**
 * Created by Crow on 2/1/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "log_stream.h"

namespace log {
std::function<void(const char *, size_t)> log_stream::output_func_ = nullptr;
log_stream::~log_stream()
{
  log::log_stream::output_func_(buffer_.data(), buffer_.count());
}

void log_stream::output_message_func(std::function<void(const char *, size_t)> functor)
{
  log_stream::output_func_ = std::move(functor);
}

template <typename T>
void format_num(log_buffer &buffer, T val)
{
  auto str = std::to_string(val);
  buffer.append(str.c_str(), str.size());
}

// 模板实例化, 缓解代码膨胀

template<int> void format_num(log_buffer, int);
template<long> void format_num(log_buffer, long);
template<long long> void format_num(log_buffer, long long);
template<unsigned int> void format_num(log_buffer, unsigned int);
template<unsigned long> void format_num(log_buffer, unsigned long);
template<long long> void format_num(log_buffer, unsigned long long);
//


log_stream &log_stream::operator<<(short val)
{
  return operator<<(static_cast<int>(val));
}

log_stream &log_stream::operator<<(unsigned short val)
{
  return operator<<(static_cast<unsigned int>(val));
}

log_stream &log_stream::operator<<(int val)
{
  format_num(buffer_, val);
  return *this;
}

log_stream &log_stream::operator<<(unsigned int val)
{
  format_num(buffer_, val);
  return *this;
}

log_stream &log_stream::operator<<(long val)
{
  format_num(buffer_, val);
  return *this;
}

log_stream &log_stream::operator<<(unsigned long val)
{
  format_num(buffer_, val);
  return *this;
}

log_stream &log_stream::operator<<(long long val)
{
  format_num(buffer_, val);
  return *this;
}

log_stream &log_stream::operator<<(unsigned long long val)
{
  format_num(buffer_, val);
  return *this;
}

log_stream &log_stream::operator<<(float val)
{
  format_num(buffer_, val);
  return *this;
}

log_stream &log_stream::operator<<(double val)
{
  format_num(buffer_, val);
  return *this;
}

}