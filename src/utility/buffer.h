/**
 * Created by Crow on 3/7/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow 
 */

#ifndef LEVELNET_NET_BUFFER_H
#define LEVELNET_NET_BUFFER_H

#include <vector>
namespace levelnet {

class Buffer {
 public:
  using const_iter = std::vector<unsigned char>::const_iterator;

  Buffer();
  ~Buffer() = default;

  size_t ReadFd(int fd);
  void Reprepare(long size);
  const_iter BufferBegin() const;
  const_iter BufferEnd() const;
 private:
  void FillData(void *data, size_t length);
  void MoveData();

  size_t Readable()        { return write_index_ - read_index_; }
  size_t Writeable()       { return buffer_.capacity() - write_index_; }
  size_t FrontSpace()      { return read_index_ - 0; }

  std::vector<unsigned char> buffer_;
  size_t read_index_;
  size_t write_index_;
};

}

#endif //LEVELNET_NET_BUFFER_H
