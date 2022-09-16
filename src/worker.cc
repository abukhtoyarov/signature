#include "worker.h"

#include <iostream>

#include "md5.h"
#include "util.h"

using namespace std;

namespace sig {

string work(char* buf, size_t sz, Worker::State& state)
{
    auto hash = calcHash(buf, sz);
    state = Worker::State::Finished;
    return hash;
}

void Worker::doWork(Block&& buf)
{
    buf_ = move(buf);
    state_ = Worker::State::Work;
    result_ = execute_async(io_, work, buf_.data(), buf_.size(), state_);
}

void Worker::cleanup()
{
    std::fill(buf_.begin(), buf_.end(), 0);
    state_ = State::Wait;
}

std::string Worker::getResult()
{
    if (result_.valid())
        return result_.get();
    else
        return "";
}

} // namespace sig
