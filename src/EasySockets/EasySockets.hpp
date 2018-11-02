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

#ifndef _EASYSOCKETS_HPP_
#define _EASYSOCKETS_HPP_

#include <memory>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace es {

enum {
  TCP = 1,
  UDP = 2,

  ERROR_NONE   = 0x00,
  ERROR_ACCEPT = 0x1A,
  ERROR_READ   = 0x2A,
  ERROR_SEND   = 0x3A,

  READ_SOME  = 0x1B,
  READ_UNTIL = 0x2B,

  ACCEPT = 0x01,
  CLOSE  = 0x02,
  READ   = 0x03,
  SEND   = 0x04,
  BEGIN  = 0x10,
  END    = 0x20,
  HANDLE = 0x30,
  
  ACCEPT_BEGIN  = ACCEPT | BEGIN,
  ACCEPT_HANDLE = ACCEPT | HANDLE,
  READ_BEGIN    = READ | BEGIN,
  READ_HANDLE   = READ | HANDLE,
  SEND_BEGIN    = SEND | BEGIN,
  SEND_HANDLE   = SEND | HANDLE,
  CLOSE_HANDLE  = CLOSE | HANDLE
};

typedef boost::asio::streambuf StreamBuffer;
typedef boost::asio::buffers_iterator<boost::asio::const_buffers_1, char> BufferIterator;

typedef boost::asio::ip::tcp::socket TCPSocket;
typedef boost::asio::ip::udp::socket UDPSocket;

typedef std::shared_ptr<boost::asio::ip::tcp::socket> TCPSocketP;
typedef std::shared_ptr<boost::asio::ip::udp::socket> UDPSocketP;

typedef std::shared_ptr<boost::asio::deadline_timer> TimerP;
typedef std::shared_ptr<boost::asio::streambuf> StreamBufferP;
typedef std::shared_ptr<boost::asio::io_service> IOServiceP;

/**
 * Returns a new id guaranteed to be unique relative to all previous
 * calls to this function.
 * 
 * @author Tyler O'Brien <contact@tylerobrien.com>
 * */
inline uint64_t make_uid()
{
  static uint64_t uid = 0;
  return uid++;
}

}

#endif