#include <boost/asio/io_service.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "arg_parser.h"
#include "config.h"
#include "context.h"
#include "processing.h"
#include "reader.h"
#include "writer.h"

using namespace std;
using namespace sig;
using namespace boost::asio;
using boost::asio::io_context;

Context get_context(int argc, const char** argv)
{
    Config config;

    config.threadNum = thread::hardware_concurrency();
    config.workerNum = max(1, config.threadNum - 2);

    if (!parse_command_line(argc, argv, config)) {
        exit(1);
    }

    cout << "Signature file generating started."
         << "\n"
         << "Input file path: " << config.inputFile << "\n"
         << "Output file path: " << config.outputFile << "\n"
         << "Block size: " << config.blockSize << " Bytes\n";

    return Context{config};
}

int main(int argc, const char** argv)
{
#if 0
    static const char* args[] = {
        "signature.exe", "-i", "in_file_name2", "-o", "out_file_name", "-b", "512", nullptr
    };
    auto ctx = get_context(sizeof(args) / sizeof(char*) - 1, args);
#else
    auto ctx = get_context(argc, argv);
#endif

    io_context io;

    // Read from file.
    Reader reader(io, ctx);
    reader.doWork();

    // Process read chunks and hash it, then pass it to writer.
    Processing proc(io, ctx);
    proc.doWork();

    // Write hashed sequency into the file.
    Writer writer(io, ctx);
    writer.doWork();

    try {
        vector<thread> pool;
        for (auto i = 0; i < max(2, ctx.config().threadNum); ++i)
            pool.push_back(thread([&io]() { io.run(); }));

        for (auto& thr : pool) thr.join();
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
    }

    cout << "[ FINISHED ]" << endl;
    return 0;
}
