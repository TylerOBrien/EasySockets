/*
* EasySockets
*
* https://tylerobrien.com
* https://github.com/TylerOBrien
*
* Copyright (c) 2018 Tyler O'Brien
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* */

#ifndef _EASYSOCKETS_TCPSERVER_HPP_
#define _EASYSOCKETS_TCPSERVER_HPP_

#include "Server.hpp"

namespace es {

class TCPServer : public Server<boost::asio::ip::tcp> {
public:
  typedef std::shared_ptr<TCPServer> Pointer;
private:
  bool __is_started;
protected:
  boost::asio::ip::tcp::acceptor _acceptor;

  /**
   * 
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  void _begin_accept() {
    TCPSocketP client = std::make_shared<TCPSocket>(*_io_service);

    _events.push(std::make_shared<Event>(
      client, es::ACCEPT_BEGIN, _protocol
    ));

    _acceptor.async_accept(*client,
      boost::bind(&TCPServer::_handle_accept,
        this, client, boost::asio::placeholders::error
      )
    );
  }

  /**
   * 
   * @param
   * @param
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  void _handle_accept(
    SocketP client,
    boost::system::error_code error)
  {
    _events.push(std::make_shared<Event>(
      client, es::ACCEPT_HANDLE, _protocol, error
    ));

    if (_auto_read) {
      _begin_read(client);
    }

	  _begin_accept();
  }
public:
  /**
   * 
   * @param 
   * @param 
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  TCPServer::TCPServer(
    const std::string& host,
    uint16_t port)
  : Server<boost::asio::ip::tcp>(es::TCP, host, port),
    __is_started(false),
    _acceptor(
      *_io_service,
      boost::asio::ip::tcp::endpoint(
        boost::asio::ip::address::from_string(host), port
      )
    )
  {}

  /**
   * 
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  UpdateResult TCPServer::update()
  {
    if (!__is_started) {
      _begin_accept();
      __is_started = true;
    }

    return Server<boost::asio::ip::tcp>::update();
  }
};

}

#endif