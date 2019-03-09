/**
 * Created by Crow on 3/2/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  
 */

#include "async_logging.h"
#include <memory>
#include <cassert>

namespace log {

async_logging::async_logging(const std::string &file_name, size_t time)
    : file_(std::make_unique<log_file>(file_name)),
      file_name_(file_name),
      time_(time * 1000000),
      running_(false),
      front_first_buffer_(std::make_unique<log_buffer>()),
      front_second_buffer_(std::make_unique<log_buffer>()),
      thread_(std::make_unique<std::thread>([this](){ append_to_file(); }))
{

}

async_logging::async_logging(const std::string &file_name)
    : async_logging(file_name, 3)
{ ; }

void async_logging::append(const char *log_message, size_t len)
{
  std::lock_guard<std::mutex> lk(mtx_);
  assert(front_first_buffer_);


  if (front_first_buffer_->available() > len) {
    front_first_buffer_->append(log_message, len);
  } else {
    buffers_.emplace_back(front_first_buffer_.release());

    if (front_second_buffer_) {
      front_first_buffer_ =
          std::move(front_second_buffer_);         //  => front_first_buffer_.reset(front_second_buffer_.release())
    } else {
      front_first_buffer_ =
          std::make_unique<log_buffer>();
    }

    front_first_buffer_->append(log_message, len);
    cond_.notify_all();
  }
}

void async_logging::append_to_file()
{
  auto buffer1 = std::make_unique<log_buffer>();
  auto buffer2 = std::make_unique<log_buffer>();
  buffer1->reset();
  buffer2->reset();
  std::vector<std::unique_ptr<log_buffer>> buffers_to_write;
  buffers_to_write.reserve(10);

  while (running_) {
    {
      std::unique_lock<std::mutex> unique_lk(mtx_);
      cond_.wait_for(unique_lk, time_, [this](){ return buffers_.empty(); });

      buffers_.emplace_back(front_first_buffer_.release());
      front_first_buffer_ = std::move(buffer1);
      buffers_to_write.swap(buffers_);

      if (!front_second_buffer_)
        front_second_buffer_ = std::move(buffer2);
    }

    if (!buffers_to_write.empty()) {
      for (const auto &var : buffers_to_write) {
        file_->flush(var->data(), var->count());
      }
    }

    if (buffers_to_write.size() > 2) {
      buffers_to_write.erase(buffers_to_write.cbegin() + 2, buffers_to_write.cend());
      buffers_to_write.resize(2);
    }

    if (!buffer1) {
      assert(!buffers_to_write.empty());
      buffer1 = std::move(buffers_to_write.back());
      buffers_to_write.pop_back();
      buffer1->reset();
    }

    if (!buffer2) {
      assert(!buffers_to_write.empty());
      buffer2 = std::move(buffers_to_write.back());
      buffers_to_write.pop_back();
      buffer2->reset();
    }

    buffers_to_write.clear();
    assert(front_first_buffer_);
    assert(front_second_buffer_);
  }
}

async_logging::~async_logging()
{
//  stop();
  thread_->join();
}

void async_logging::start()
{
  running_ = true;
}

void async_logging::stop()
{
  running_ = false;
}

}