#ifndef READER_H
#define READER_H
#pragma once
#include <boost/asio/io_context.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "config.h"
#include "context.h"
#include "file.h"

namespace sig {

class Reader {
  Reader() = delete;
  Reader(Reader&&) = delete;
  Reader(const Reader&) = delete;

 public:
  Reader(::boost::asio::io_context& io, Context& ctx) : io_(io), ctx_(ctx) {}

  void doWork();

 protected:
  void openInputFile();
  void work();

 private:
  ::boost::asio::io_context& io_;
  Context& ctx_;
  File file_;
};

}  // namespace sig

#endif
