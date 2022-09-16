#include "processing.h"

#include <boost/bind/bind.hpp>

#include "config.h"

using namespace std;
using namespace boost::asio;
using ::boost::bind;

namespace sig {

    void Processing::spawnWorkers()
    {
        workerNum = ctx_.config().workerNum;

        while (i < workerNum && isAnyData) {
            MessageBlock msg;
            ctx_.input().pop(msg);

            switch (msg.state) {
                case State::Ok:
                    workers_.emplace_back(io_, ctx_.config().blockSize);
                    workers_.back().doWork(move(msg.data));
                    ++i;
                    break;

                case State::Exit: {
                    workerNum = i;
                    isAnyData = false;
                    break;
                }

                case State::Error:
                default:
                    cerr << "[Processing] Reader failed" << endl;
                    terminate();
            }
        }
    };

    bool Processing::processResult(Worker& worker)
    {
        switch (worker.state()) {
            case Worker::State::Finished:
            case Worker::State::Work: {
                auto result = worker.getResult();
                ctx_.output().push({State::Ok, result});
            } break;

            case Worker::State::Wait: {
                cout << "[Processing] Last data block processed" << endl;
                ctx_.output().push({State::Exit, {}});
                return false;
            } break;
        }

        worker.cleanup();
        return true;
    }

    void Processing::consume(Worker& worker)
    {
        MessageBlock msg;
        ctx_.input().pop(msg);

        switch (msg.state) {
            case State::Ok:
                worker.doWork(move(msg.data));
                break;

            case State::Exit:
                cout << "[Processing] Last data block to processing" << endl;
                isAnyData = false;
                break;

            case State::Error:
            default:
                cerr << "[Processing] Reader failed" << endl;
                terminate();
        }
    }

    void Processing::work()
    {
        spawnWorkers();

        while (true) {
            auto& worker = workers_[i % workerNum];

            if (!processResult(worker)) break;

            // Is there new piece of data to processing?
            if (isAnyData) consume(worker);

            ++i;
        }

        cout << "[Processing] finished" << endl;
    }

    void Processing::doWork() { io_.post(bind(&Processing::work, this)); }

}  // namespace sig
