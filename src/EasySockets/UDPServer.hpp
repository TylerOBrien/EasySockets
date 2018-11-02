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

#ifndef _EASYSOCKETS_UDPSERVER_HPP_
#define _EASYSOCKETS_UDPSERVER_HPP_

#include "Server.hpp"

namespace es {

class UDPServer : public Server<boost::asio::ip::udp> {
public:
  typedef std::shared_ptr<UDPServer> Pointer;
public:
  /**
   * 
   * @param 
   * @param 
   * 
   * @author Tyler O'Brien <contact@tylerobrien.com>
   * */
  UDPServer::UDPServer(
    const std::string& host,
    uint16_t port)
  : Server<boost::asio::ip::udp>(es::UDP, host, port)
  {}
};

}

#endif