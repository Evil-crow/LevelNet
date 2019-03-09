/**
 * Created by Crow on 3/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_WRITE_TASK_H
#define LEVELNET_WRITE_TASK_H

#include <cstdio>
#include <memory>

namespace levelnet {

class WriteTask {
 public:
  WriteTask() = default;
  WriteTask(int fd, const unsigned char *data, off64_t completed, size_t total);
  ~WriteTask() = default;
  bool operator()();

 protected:
  int fd_{};
  size_t total_{};
  size_t remained_{};
  off_t completed_{};
  std::shared_ptr<unsigned char> data_;
};

}

#endif //LEVELNET_WRITE_TASK_H
