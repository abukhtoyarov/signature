#ifndef FILE_H
#define FILE_H
#pragma once
#include <boost/asio/io_context.hpp>
#include <string>
#include <vector>

#if (WIN32)

#include <boost/asio.hpp>
using Stream = ::boost::asio::windows::random_access_handle;

#endif

#ifdef __unix__

#include <boost/asio/posix/stream_descriptor.hpp>
using Stream = boost::asio::posix::stream_descriptor;

#endif

namespace sig {

enum class Disposition { Read,
    Write };

class File {
public:
    ~File();

    void open(const std::string& file, Disposition d);
    Stream stream(boost::asio::io_context& io) const;
    size_t read(Stream& stm, std::vector<char>& buf,
        boost::system::error_code& err);
    size_t write(Stream& stm, std::string& buf, boost::system::error_code& err);

private:
#if (WIN32)
    size_t offset_ = 0;
    HANDLE hndl_;
#endif

#ifdef __unix__
    int fd_ = -1;
#endif
};

} // namespace sig

#endif
