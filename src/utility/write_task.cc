/**
 * Created by Crow on 3/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "write_task.h"

#include <unistd.h>
#include <memory>

#include "utility/logger/logger.h"

namespace levelnet {

WriteTask::WriteTask(int fd,
                     const unsigned char *data,
                     off64_t completed,
                     size_t total)
  : fd_(fd),
    total_(total),
    remained_(total),
    completed_(completed)
{
  data_ = std::shared_ptr<unsigned char[]>(
      new unsigned char[total],
      std::default_delete<unsigned char[]>());
}

bool WriteTask::operator()() {
  while (true) {
    auto var = ::write(fd_, data_.get() + completed_, remained_);
    if (var == remained_) {
      return true;
    } else if ( var >= 0 && var < remained_) {
      remained_ -= var;
      completed_ += var;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return false;
      } else {
        LOG(ERROR) << "WriteTask::operator() => Write Error";
        std::abort();
      }
    }
  }
}

}