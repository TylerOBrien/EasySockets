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

#ifndef _EASYSOCKETS_TYPES_HPP_
#define _EASYSOCKETS_TYPES_HPP_

#include <memory>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace es {

typedef boost::asio::streambuf StreamBuffer;
typedef boost::asio::buffers_iterator<boost::asio::const_buffers_1, char> BufferIterator;

typedef boost::asio::ip::tcp::socket TCPSocket;
typedef boost::asio::ip::udp::socket UDPSocket;

typedef std::shared_ptr<boost::asio::ip::tcp::socket> TCPSocketP;
typedef std::shared_ptr<boost::asio::ip::udp::socket> UDPSocketP;

typedef std::shared_ptr<boost::asio::deadline_timer> TimerP;
typedef std::shared_ptr<boost::asio::streambuf> StreamBufferP;
typedef std::shared_ptr<boost::asio::io_service> IOServiceP;

}

#endif