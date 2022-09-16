#include "writer.h"

#include <boost/asio/write.hpp>
#include <boost/bind/bind.hpp>

#include "context.h"
#include "util.h"
#include "worker.h"

using namespace std;
using namespace boost::asio;
using namespace boost::system::errc;
using ::boost::bind;

namespace sig {

void Writer::work()
{
    auto stream = file_.stream(io_);

    auto write = [&](auto& data) {
        boost::system::error_code err;
        auto wrote = file_.write(stream, data, err);
        switch (err.value()) {
        case success:
            break;

        default: {
            std::string e = "[Writer] [ ERROR ] Code: " + std::to_string(err.value()) + " Description: " + err.what();
            std::cerr << e << "\n";
            terminate();
        } break;
        }
    };

    while (true) {
        MessageHash msg;
        ctx_.output().pop(msg);

        switch (msg.state) {
        case State::Ok:
            write(msg.data);
            break;

        case State::Exit:
            cout << "[Writer] Finished"
                 << "\n";
            return;

        case State::Error:
        default:
            cerr << "[Writer] Processing error"
                 << "\n";
            terminate();
        }
    }
}

void Writer::openOutputFile()
{
    try {
        file_.open(ctx_.config().outputFile, Disposition::Write);
    } catch (std::exception& exc) {
        cerr << exc.what() << "\n";
        cerr << "[Writer] Reader failed" << endl;
        terminate();
    }
}

void Writer::doWork()
{
    openOutputFile();
    io_.post(bind(&Writer::work, this));
}

} // namespace sig
