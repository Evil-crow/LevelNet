/**
 * Created by Crow on 2/1/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "log_stream.h"

namespace log {

template <typename T>
void format_num(log_buffer &buffer, T val)
{
  auto str = std::to_string(val);
  buffer.append(str.c_str(), str.size());
}

// 模板特例化, 缓解代码膨胀

template void format_num(log_buffer, int);
template void format_num(log_buffer, long);
template void format_num(log_buffer, long long);
template void format_num(log_buffer, unsigned int);
template void format_num(log_buffer, unsigned long);
template void format_num(log_buffer, unsigned long long);
template void format_num(log_buffer, float);
template void format_num(log_buffer, double);

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