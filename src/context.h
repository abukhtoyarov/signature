#ifndef PIPELINE_H
#define PIPELINE_H
#pragma once
#include <boost/circular_buffer.hpp>
#include <condition_variable>
#include <mutex>
#include <vector>

#include "config.h"

namespace sig {

using Block = std::vector<char>;
using Hash = std::string;

enum class State {
    Ok = 0,
    Exit,
    Error,
};

struct MessageBlock {
    State state = State::Ok;
    Block data;
};

struct MessageHash {
    State state = State::Ok;
    Hash data;
};

template <typename T>
class Queue {
    using Lock = std::unique_lock<std::mutex>;
    using Buffer = ::boost::circular_buffer<T>;

public:
    Queue(size_t capacity) { buf_.set_capacity(capacity); }

    void push(const T& block)
    {
        Lock lck(mtx_);

        while (buf_.full())
            full_.wait(lck);

        buf_.push_back(block);
        lck.unlock();
        empty_.notify_one();
    }

    void pop(T& block)
    {
        Lock lck(mtx_);

        while (buf_.empty())
            empty_.wait(lck);

        block = buf_.front();
        buf_.pop_front();

        lck.unlock();
        full_.notify_one();
    }

private:
    std::mutex mtx_;
    std::condition_variable empty_;
    std::condition_variable full_;
    Buffer buf_;
};

class Context {
public:
    Context(Config& cfg)
        : config_(cfg)
        , input_(cfg.workerNum * 4)
        , output_(cfg.workerNum * 4)
    {
    }

    Config& config() { return config_; }
    Queue<MessageBlock>& input() { return input_; }
    Queue<MessageHash>& output() { return output_; }

private:
    Config config_;
    Queue<MessageBlock> input_;
    Queue<MessageHash> output_;
};

} // namespace sig

#endif
