#ifndef PROCESSING_H
#define PROCESSING_H
#pragma once
#include <boost/asio/io_context.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "config.h"
#include "context.h"
#include "worker.h"

namespace sig {

class Processing {
public:
    Processing(::boost::asio::io_context& io, Context& ctx)
        : io_(io)
        , ctx_(ctx)
    {
    }

    void doWork();

protected:
    void spawnWorkers();
    bool processResult(Worker& worker);
    void consume(Worker& worker);

    void work();

private:
    ::boost::asio::io_context& io_;
    Context& ctx_;

    unsigned spawned_ = 0;
    std::vector<Worker> workers_;

    unsigned i = 0;
    bool isAnyData = true;
    bool exit = false;
    unsigned workerNum = 0;
};

} // namespace sig

#endif