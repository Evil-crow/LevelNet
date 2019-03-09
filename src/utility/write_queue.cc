/**
 * Created by Crow on 3/9/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#include "write_queue.h"

#include "write_task.h"

namespace levelnet {

void WriteQueue::TaskInQueue(int fd, const unsigned char *data, off64_t completed, size_t total) {
  queue_.emplace_back(std::make_shared<WriteTask>(fd, data, completed, total));
}

void WriteQueue::TasKInQueue(int fd, const std::string &data, off64_t completed) {
  TaskInQueue(fd, reinterpret_cast<unsigned char *>(data.c_str()), completed, data.size());
}

bool WriteQueue::DoTask() {
  while (!queue_.empty()) {                        // if we complete all the task, we should get out.
    if (queue_.front()->operator()()) {            // complete the task, std::queue::front() => reference, this can change the state of the elements in the container.
      queue_.pop_front();
    } else {                            // non-complete the task, e.g. EAGAIN / EWOULDBLOCK
      break;
    }
  }
  return queue_.empty();
}

}