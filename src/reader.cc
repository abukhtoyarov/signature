#include "reader.h"
#include <boost/asio/read.hpp>
#include <boost/bind/bind.hpp>
#include <thread>
#include <chrono>

using namespace std;
using boost::asio::buffer;
using boost::asio::transfer_at_least;
using namespace boost::asio;
using ::boost::bind;

namespace sig {

void Reader::openInputFile() {
    try {
        file_.open(ctx_.config().inputFile, Disposition::Read);
    }
    catch (std::exception& exc) {
        cerr << exc.what() << "\n";
        cerr << "[Reader] Reader failed" << endl;
        terminate();
    }
}

void Reader::work() {
    auto stremDescriptor = file_.stream(io_);
    std::vector<char> buf(ctx_.config().blockSize);

    auto read = [&] () -> bool {
        std::fill(buf.begin(), buf.end(), 0);

        boost::system::error_code err;
        auto rd = file_.read(stremDescriptor, buf, err);
        switch (err.value()) {
            case ::boost::system::errc::success:
                ctx_.input().push({ State::Ok, buf });
                return true;

            case error::misc_errors::eof: {
                if (rd)
                    ctx_.input().push({ State::Ok, buf });

                ctx_.input().push({ State::Exit, {} });
                return false;
            }

            default: {
                string e = "[Reader] [ ERROR ] Code: " + to_string(err.value()) + " Description: " + err.what();
                cerr << e << "\n";
                terminate();
            }
        }
    };

    while (true)
        if (!read())
            break;
}

void Reader::doWork() {
    openInputFile();
    io_.post(bind(&Reader::work, this));
}

} // namespace sig
