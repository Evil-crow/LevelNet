/**
 * Created by Crow on 3/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_WRITE_QUEUE_H
#define LEVELNET_WRITE_QUEUE_H

#include <cstdio>
#include <memory>
#include <deque>

namespace levelnet {

class WriteTask;
class WriteQueue {
 public:
  void TaskInQueue(int fd, const unsigned char *data, off64_t completed, size_t total);
  void TasKInQueue(int fd, const std::string &data, off64_t completed);
  bool DoTask();
 private:
  // in order to avoid STL memory leak => shared_ptr, NOT unique_ptr ? think the essence of STL behavior
  std::deque<std::shared_ptr<WriteTask>> queue_;
};

}

#endif //LEVELNET_WRITE_QUEUE_H
