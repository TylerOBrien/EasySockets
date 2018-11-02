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

#ifndef _EASYSOCKETS_SERVER_HPP_
#define _EASYSOCKETS_SERVER_HPP_

#include "Event.hpp"
#include "Logger.hpp"

#include <functional>
#include <queue>

namespace es {

class UpdateResult {
protected:
  /**
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * todo:
   *  - Move this into the server class itself. Otherwise having more than one Server object will cause multiple calls here and muck things up.
   * */
  static std::chrono::milliseconds _get_delta() {
    static std::chrono::system_clock::time_point previous_when = std::chrono::system_clock::now();
    
    std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(
      previous_when - std::chrono::system_clock::now()
    );

    previous_when = std::chrono::system_clock::now();

    return delta;
  }
public:
  std::size_t nhandles_executed;
  int8_t protocol;
  boost::system::error_code error;
  std::chrono::milliseconds delta;

  UpdateResult(
    std::size_t nhandles_executed,
    int8_t protocol,
    boost::system::error_code error)
    : nhandles_executed(nhandles_executed),
      protocol(protocol),
      error(error),
      delta(UpdateResult::_get_delta())
  {}
};

template <
  class ProtocolTy,
  class _LoggerTy = Logger>
class Server {
public:
  typedef std::shared_ptr<Server<ProtocolTy>> Pointer;
  typedef std::shared_ptr<typename ProtocolTy::socket> SocketP;
protected:
  _LoggerTy _logger;

  bool _auto_read;
  
  std::function<void()> _io_service_run_one;
  std::function<std::size_t()> _io_service_poll;

  int8_t _protocol;
  int8_t _read_mode;
  std::size_t _read_buffer_nbytes;
  std::string _read_delimeter;
  IOServiceP _io_service;
  std::queue<EventP> _events;

  /**
   * Starts receiving from the passed socket using the passed string as a
   * delimeter. The socket will remain in a transmission state until the
   * passed delimeter is received, the action is cancelled, or the socket
   * is forcibly closed.
   * 
   * @param client The socket connection.
   * @param delim The delimeter denoting when upon delivery to stop receiving.
   *
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  void _begin_read_until(
    SocketP client,
    const std::string& delim,
    uint64_t event_id)
  {
    StreamBufferP buffer = std::make_shared<StreamBuffer>();

    boost::asio::async_read_until(
      *client, *buffer, delim.c_str(),
      boost::bind(&Server::_handle_read,
        this, client, buffer, event_id,
        boost::asio::placeholders::bytes_transferred,
        boost::asio::placeholders::error
      )
    );
  }

  /**
   * Starts receiving from the passed socket using the set default number
   * of bytes as a buffer size. The socket will remain in a transmission state until the
   * buffer is filled (i.e. the set number of bytes are received,) the action is
   * cancelled, or the socket is forcibly closed.
   * 
   * @param client The socket connection.
   *
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  void _begin_read_some(
    SocketP client,
    uint64_t event_id)
  {
    StreamBufferP buffer = std::make_shared<StreamBuffer>(_read_buffer_nbytes);

    boost::asio::async_read(
      *client, *buffer,
      boost::bind(&Server::_handle_read,
        this, client, buffer, event_id,
        boost::asio::placeholders::bytes_transferred,
        boost::asio::placeholders::error
      )
    );
  }

  /**
   * 
   * 
   * @param client The socket connection.
   * @param buffer 
   * @param nbytes_received The number of bytes received.
   * @param error The error container. Expected generic/blank if there was no error.
   *
   * @author Tyler O'Brien <contact@tylerobrien.com>
   *
   * todo:
   *  - Implement read timeout event.
   * */
  void _handle_read(
    SocketP client,
    StreamBufferP buffer,
    uint64_t unique_id,
    std::size_t nbytes_received,
    boost::system::error_code error)
  {
    if (buffer->size()) {
      _events.push(std::make_shared<ReadEvent>(
          buffer, nbytes_received, client, es::READ_HANDLE, _protocol, unique_id, error
      ));

      if (_auto_read) {
        _begin_read(client);
      }
    } else {
      _handle_close(client);
    }
  }

  /**
   * Called when the passed socket's connection has been closed.
   *
   * @param client The socket connection.
   *
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  void _handle_close(
    SocketP client)
  {
    _events.push(std::make_shared<Event>(
      client, es::CLOSE_HANDLE, _protocol
    ));
  }


  /**
   * 
   * 
   * @param The socket connection.
   * @param
   * @param
   * @param
   *
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  void _handle_send(
    SocketP client,
    uint64_t transfer_id,
    std::size_t nbytes_sent,
    boost::system::error_code error)
  {
    _events.push(std::make_shared<SendEvent>(
      transfer_id, nbytes_sent, client, es::SEND_HANDLE, _protocol, error
    ));
  }
public:
  /**
   * 
   * 
   * @param
   * @param
   * @param
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  Server(
    int8_t protocol,
    const std::string& host,
    uint16_t port)
  : _auto_read(true),
    _protocol(protocol),
    _io_service(std::make_shared<boost::asio::io_service>())
  {}

  /**
   * 
   * 
   * @param
   * @param
   * @param
   * @param
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  Server(
    int8_t protocol,
    IOServiceP io_service,
    const std::string& host,
    uint16_t port)
  : _auto_read(true),
    _io_service(io_service),
    _protocol(protocol)
  {}

  /**
   * Returns the most recent event, if there is one. If there is no event
   * a "blank" event is returned that will be falsey.
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  EventP poll()
  {
    if (!_events.empty()) {
      EventP event = _events.front();
      _events.pop();
      return event;
    }

    return EventP();
  }
  
  /**
   * 
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  UpdateResult update()
  {
    boost::system::error_code error;

    _io_service->run_one();
    
    std::size_t nhandles_executed = _io_service->poll(error);

    return UpdateResult(nhandles_executed, _protocol, error);
  }
  
  /**
   * Called when it is needed to receive data from the passed socket.
   * 
   * @param client The socket connection.
   *
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  uint64_t _begin_read(
    SocketP client)
  {
    uint64_t event_id = es::make_uid();

    _events.push(std::make_shared<Event>(
      client, es::READ_BEGIN, _protocol, event_id
    ));

    if (_read_mode == es::READ_UNTIL) {
      _begin_read_until(client, _read_delimeter, event_id);
    } else {
      _begin_read_some(client, event_id);
    }

    return event_id;
  }
  
  /**
   * Called when it is needed to receive data from the passed socket for a
   * predefined number of bytes.
   * 
   * @param client The socket connection.
   * @param nbytes The number of bytes to receive.
   *
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  uint64_t _begin_read_nbytes(
    SocketP socket,
    std::size_t nbytes)
  {
    int8_t previous_read_mode = _read_mode;
    std::size_t previous_nbytes = _read_buffer_nbytes;

    set_read_mode(es::READ_SOME);
    set_read_buffer_nbytes(nbytes);

    uint64_t event_id = _begin_read(socket);

    set_read_mode(previous_read_mode);
    set_read_buffer_nbytes(previous_nbytes);

    return event_id;
  }

  /**
   * 
   * 
   * @param The socket connection.
   * @param
   *
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  uint64_t sendb(
    SocketP client,
    StreamBufferP payload)
  {
    uint64_t event_id = es::make_uid();

    _events.push(std::make_shared<SendEvent>(
      uid, event_id, client, es::SEND_BEGIN, _protocol
    ));

    boost::asio::async_write(
      *client, *payload,
      boost::bind(&Server::_handle_send,
        this, client, event_id,
        boost::asio::placeholders::bytes_transferred,
        boost::asio::placeholders::error
      )
    );

    return event_id;
  }

  /**
   * 
   * 
   * @param The socket connection.
   * @param
   *
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  uint64_t sends(
    SocketP client,
    const std::string& payload)
  {
    uint64_t event_id = es::make_uid();

    _events.push(std::make_shared<SendEvent>(
      event_id, 0, client, es::SEND_BEGIN, _protocol
    ));

    boost::asio::async_write(
      *client, boost::asio::buffer(payload.c_str(), payload.size()),
      boost::bind(&Server::_handle_send,
        this, client, event_id,
        boost::asio::placeholders::bytes_transferred,
        boost::asio::placeholders::error
      )
    );

    return event_id;
  }

  /**
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  void set_read_timeout_seconds(
    uint16_t seconds)
  {
    _read_timeout_seconds = seconds;
  }

  /**
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  void set_read_mode(
    int8_t mode)
  {
    _read_mode = mode;
  }

  /**
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  void set_read_delimeter(
    const std::string& delimeter)
  {
    _read_delimeter = delimeter;
  }

  /**
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  void set_read_buffer_nbytes(
    std::size_t nbytes)
  {
    _read_buffer_nbytes = nbytes;
  }
};

}

#endif