#ifndef UTIL_H
#define UTIL_H
#include <boost/asio/post.hpp>
#include <future>

namespace sig {

template <typename F, typename... Args>
inline std::future<std::string> execute_async(boost::asio::io_context& io,
                                              F&& f, Args&&... args) {
  auto task = std::packaged_task<std::string()>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));
  std::future<std::string> res = task.get_future();

  boost::asio::post(io, std::bind(std::move(task)));
  return res;
}

}  // namespace sig

#endif
