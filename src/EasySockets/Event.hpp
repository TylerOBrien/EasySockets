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

#ifndef _EASYSOCKETS_EVENT_HPP_
#define _EASYSOCKETS_EVENT_HPP_

#include "EasySockets.hpp"
#include "Error.hpp"

#include <chrono>

namespace es {

class Event
{
protected:
  std::shared_ptr<void> _target;
public:
  int type;
  int8_t protocol;
  uint64_t uid;
  Error error;
  std::chrono::system_clock::time_point when;

  /**
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  explicit operator bool () const {
    return type != 0;
  }

  /**
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  bool operator ! () const {
    return type == 0;
  }

  /**
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  Event()
    : type(0),
      protocol(0),
      uid(es::make_uid()),
      when(std::chrono::system_clock::now())
  {}

  /**
   * 
   * 
   * @param
   * @param
   * @param
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  template <class SocketPTy>
  Event(
    SocketPTy target,
    int type,
    int8_t protocol)
    : _target(std::static_pointer_cast<void>(target)),
      type(type),
      protocol(protocol),
      uid(es::make_uid()),
      when(std::chrono::system_clock::now())
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
  template <class SocketPTy>
  Event(
    SocketPTy target,
    int type,
    int8_t protocol,
    uint64_t unique_id)
    : _target(std::static_pointer_cast<void>(target)),
      type(type),
      protocol(protocol),
      uid(unique_id),
      when(std::chrono::system_clock::now())
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
  template <class SocketPTy>
  Event(
    SocketPTy target,
    int type,
    int8_t protocol,
    boost::system::error_code error)
    : _target(std::static_pointer_cast<void>(target)),
      type(type),
      protocol(protocol),
      uid(es::make_uid()),
      when(std::chrono::system_clock::now())
  {}

  /**
   * 
   * 
   * @param
   * @param
   * @param
   * @param
   * @param
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  template <class SocketPTy>
  Event(
    SocketPTy target,
    int type,
    int8_t protocol,
    uint64_t unique_id,
    boost::system::error_code error)
    : _target(std::static_pointer_cast<void>(target)),
      type(type),
      protocol(protocol),
      uid(unique_id),
      when(std::chrono::system_clock::now())
  {}

  /**
   * 
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  template <class SocketTy>
  std::shared_ptr<SocketTy> socket() const {
    return std::static_pointer_cast<SocketTy>(_target);
  }
};

class ReadEvent : public Event {
public:
  StreamBufferP buffer;
  std::size_t nbytes_transferred;

  /**
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  ReadEvent()
    : Event(), nbytes_transferred(0)
  {}

  /**
   * 
   * @param 
   * @param 
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  ReadEvent(
    StreamBufferP buffer,
    std::size_t nbytes_transferred)
    : Event(),
      buffer(buffer),
      nbytes_transferred(nbytes_transferred)
  {}

  /**
   * 
   * @param 
   * @param 
   * @param 
   * @param 
   * @param 
   * @param 
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  template <class SocketPTy>
  ReadEvent(
    StreamBufferP buffer,
    std::size_t nbytes_transferred,
    SocketPTy target,
    int type,
    int8_t protocol,
    boost::system::error_code error)
    : Event(target, type, protocol, error),
      buffer(buffer),
      nbytes_transferred(nbytes_transferred)
  {}

  /**
   * 
   * @param 
   * @param 
   * @param 
   * @param 
   * @param 
   * @param 
   * @param 
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  template <class SocketPTy>
  ReadEvent(
    StreamBufferP buffer,
    std::size_t nbytes_transferred,
    SocketPTy target,
    int type,
    int8_t protocol,
    uint64_t unique_id,
    boost::system::error_code error)
    : Event(target, type, protocol, unique_id, error),
      buffer(buffer),
      nbytes_transferred(nbytes_transferred)
  {}
};

class SendEvent : public Event {
public:
  uint64_t transfer_id;
  std::size_t nbytes_transferred;

  /**
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  SendEvent()
    : Event(),
      transfer_id(0),
      nbytes_transferred(0)
  {}

  /**
   * 
   * 
   * @param 
   * @param 
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  SendEvent(
    uint64_t transfer_id,
    std::size_t nbytes_transferred)
    : Event(),
      transfer_id(transfer_id),
      nbytes_transferred(nbytes_transferred)
  {}

  /**
   * 
   * 
   * @param
   * @param
   * @param
   * @param
   * @param
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  template <class SocketPTy>
  SendEvent(
    uint64_t transfer_id,
    std::size_t nbytes_transferred,
    SocketPTy target,
    int type,
    int8_t protocol)
    : Event(target, type, protocol),
      transfer_id(transfer_id),
      nbytes_transferred(nbytes_transferred)
  {}

  /**
   * 
   * 
   * @param
   * @param
   * @param
   * @param
   * @param
   * @param
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  template <class SocketPTy>
  SendEvent(
    uint64_t transfer_id,
    std::size_t nbytes_transferred,
    SocketPTy target,
    int type,
    int8_t protocol,
    boost::system::error_code error)
    : Event(target, type, protocol, error),
      transfer_id(transfer_id),
      nbytes_transferred(nbytes_transferred)
  {}
};

typedef std::shared_ptr<Event> EventP;
typedef std::shared_ptr<ReadEvent> ReadEventP;
typedef std::shared_ptr<SendEvent> SendEventP;

}

#endif