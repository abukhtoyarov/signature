#ifndef WORKER_H
#define WORKER_H
#pragma once
#include <boost/asio/io_context.hpp>
#include <future>
#include <string>
#include <vector>

#include "config.h"
#include "context.h"

namespace sig {

class Worker {
 public:
  enum class State { Wait, Work, Finished };

 public:
  Worker(::boost::asio::io_context& io, size_t blockSize)
      : io_(io), buf_(blockSize) {}

  void doWork(Block&& buf);
  std::string getResult();
  void cleanup();
  State state() { return state_; }

 private:
  ::boost::asio::io_context& io_;

  std::future<std::string> result_;
  State state_ = State::Wait;
  Block buf_;
};

}  // namespace sig

#endif