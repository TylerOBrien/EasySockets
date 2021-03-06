# EasySockets
Lightweight framework that loosely wraps a few functions from Boost's ASIO library, primarly the [asio::async_read](https://www.boost.org/doc/libs/1_68_0/doc/html/boost_asio/reference/async_read.html), [asio::async_write](https://www.boost.org/doc/libs/1_68_0/doc/html/boost_asio/reference/async_read.html), and [asio::ip::tcp::acceptor::async_accept](https://www.boost.org/doc/libs/1_68_0/doc/html/boost_asio/reference/basic_socket_acceptor/async_accept/overload1.html) functions.

# Example
```cpp
#include "EasySockets/TCPServer.hpp"

#include <iostream>

void handle_accept(es::EventP event)
{
  std::cout
    << event->socket<es::TCPSocket>()
    << " connected"
    << std::endl;
}

void handle_read(es::EventP event)
{
  es::ReadEventP r_event = std::static_pointer_cast<es::ReadEvent>(event);
  es::BufferIterator begin = boost::asio::buffers_begin(r_event->buffer->data());
  es::BufferIterator end = boost::asio::buffers_end(r_event->buffer->data());
  
  std::cout
    << event->socket<es::TCPSocket>()
    << " sent: "
    << std::string(begin, end)
    << std::endl;
}

int main()
{
  es::TCPServer server("127.0.0.1", 5000);
  
  for (;;) {
    server.update();
    while (es::EventP event = server.poll()) {
      switch (event->type) {
      case es::ACCEPT_HANDLE:
        handle_accept(event);
        break;
      case es::READ_HANDLE:
        handle_read(event);
        break;
      }
    }
  }
}

```
