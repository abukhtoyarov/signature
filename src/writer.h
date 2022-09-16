#ifndef WRITER_H
#define WRITER_H
#pragma once
#include <boost/asio/io_context.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "config.h"
#include "context.h"
#include "file.h"

namespace sig {

class Writer {
public:
    Writer(::boost::asio::io_context& io, Context& ctx)
        : io_(io),
        ctx_(ctx)
    {}

    void doWork();

protected:
    void openOutputFile();
    void work();

private:
    ::boost::asio::io_context& io_;
    Context& ctx_;
    File file_;
};

} // namespace sig

#endif
