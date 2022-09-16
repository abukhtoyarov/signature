#include "file.h"

#include <boost/asio.hpp>

using namespace std;
using namespace std;
using ::boost::asio::io_context;

namespace sig {

#if (WIN32)

#include <Windows.h>

    void File::open(const string& file, Disposition d)
    {
        hndl_ = CreateFile(
            file.c_str(), d == Disposition::Read ? GENERIC_READ : GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
            d == Disposition::Read ? OPEN_EXISTING : CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

        if (hndl_ == INVALID_HANDLE_VALUE) {
            auto err = GetLastError();
            throw runtime_error("Failed to open file '" + file + "': " + to_string(err));
        }
    }

    Stream File::stream(io_context& io) const
    {
        size_t offset_ = 0;
        return {io, hndl_};
    }

    size_t File::read(Stream& stm, vector<char>& buf, boost::system::error_code& err)
    {
        auto rd = ::boost::asio::read_at(
            stm, offset_, ::boost::asio::buffer(buf.data(), buf.size()),
            ::boost::asio::transfer_at_least(buf.size()), err);
        offset_ += rd;
        return rd;
    }

    size_t File::write(Stream& stm, string& buf, boost::system::error_code& err)
    {
        auto wrote = ::boost::asio::write_at(
            stm, offset_, ::boost::asio::buffer(buf.data(), buf.size()), err);

        offset_ += wrote;
        return wrote;
    }

    File::~File() {}

#endif

#ifdef __unix__

#include <unistd.h>

    void File::open(const string& file, Disposition d)
    {
        fd_ = ::open(file.c_str(), d == Disposition::Read ? O_RDONLY : O_WRONLY | O_CREAT, 0666);
        if (fd_ == -1) throw runtime_error("Failed to open file " + file);
    }

    boost::asio::posix::stream_descriptor File::stream(io_context& io) const { return {io, fd_}; }

    size_t File::read(Stream& stm, vector<char>& buf, boost::system::error_code& err)
    {
        return ::boost::asio::read(
            stm, ::boost::asio::buffer(buf.data(), buf.size()),
            ::boost::asio::transfer_at_least(buf.size()), err);
    }

    size_t File::write(Stream& stm, string& buf, boost::system::error_code& err)
    {
        return ::boost::asio::write(stm, ::boost::asio::buffer(buf.data(), buf.size()), err);
    }

    File::~File()
    {
        if (fd_ > 0) ::close(fd_);
    }

#endif

}  // namespace sig
