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

#ifndef _EASYSOCKETS_ERROR_HPP_
#define _EASYSOCKETS_ERROR_HPP_

#include <boost/system/error_code.hpp>

namespace es {

class Error {
public:
  int16_t state;
  boost::system::error_code code;

  /**
   * 
   * */
  explicit operator bool () const {
    return !!code;
  }

  /**
   * 
   * */
  bool operator ! () const {
    return !code;
  }
  
  /**
   * 
   * */
  Error() : state(0) {}
  Error(int16_t state) : state(state) {}

  /**
   * 
   * */
  Error(int16_t state, boost::system::error_code code)
    : state(state), code(code)
  {}
};

}

#endif