/*
  PString.h - Lightweight printable string class
  Copyright (c) 2009-2012 Mikal Hart.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/**
 * \file PString.h
 * \brief Proveds the amazing PString class
 */
#ifndef PString_h
#define PString_h

#include "Print.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define PSTRING_LIBRARY_VERSION 3
/**
 * \class PString
 * \brief A Lightweight String Class for Formatting Text
 */
class PString : public Print
{
private:
  char *_buf, *_cur;
  size_t _size;
  bool _weOwn;
public:
#if defined(ARDUINO) && ARDUINO >= 100
	virtual size_t write(uint8_t);
#else
	virtual void write(uint8_t);
#endif

public:

  /** Basic constructor GgR - removed requirment for buffer (allowing empty Constructor)
   * \param[out] buffer Buffer to write to.
   * \param[in] size Size of write buffer.
   * \remark set Buffer=NULL and size > 0 and PString will allocate from Ram and free on destroy.
   */
  PString(char *buffer=NULL, size_t size=0) : _buf(buffer), _size(size), _weOwn(false)
  { if ((buffer==NULL) && (size > 0)) setBuffer(buffer, size); else begin(); }

  /** 
   * \brief Frees buffer if we own it.
   */
   ~PString()
   { if(_weOwn) free(_buf); }
   
  /** allows you to set a new buffer.
   * \param[out] buffer New buffer to write to.
   * \param[in] size Size of write buffer;
   * \remark set Buffer=NULL and size > 0 and PString will allocate from Ram and free on destroy.
  */
  void setBuffer(char *buffer, size_t size);
  
  /** templated constructors allow inline renderings of this type: PString(buf, size, myfloat[, modifier]); */
  template<class T> PString(char *buffer, size_t size, T arg) : _buf(buffer), _size(size), _weOwn(false) 
  { setBuffer(buffer, size); print(arg); }
  
  /** templated constructors allow inline renderings of this type: PString(buf, size, myfloat[, modifier]); */
  template<class T> PString(char *buffer, size_t size, T arg, int modifier) : _buf(buffer), _size(size), _weOwn(false)
  { setBuffer(buffer, size); print(arg, modifier); }

  /** returns the length of the current string, not counting the 0 terminator */
  inline const size_t length() 
  { return _cur - _buf; }

  /** returns the capacity of the string */
  inline const size_t capacity() 
  { return _size; }

  /** gives access to the internal string */
  inline operator const char *() 
  { return _buf; }

  /** compare to another string */
  bool operator==(const char *str) 
  { return _size > 0 && !strcmp(_buf, str); }

  /** call this to re-use an existing string */
  void begin();

  /** This function allows assignment to an arbitrary scalar value like str = myfloat; */
  template<class T> inline PString &operator =(T arg) 
  { begin(); print(arg); return *this; }

  /** Concatenation str += myfloat; */
  template<class T> inline PString &operator +=(T arg) 
  { print(arg); return *this; }

  /** Safe access to sprintf-like formatting, e.g. str.format("Hi, my name is %s and I'm %d years old", name, age); */
  int format(char *str, ...);
  
  /** format - add support for __FlashStringHelper */
  int format(const __FlashStringHelper *str, ...);
  
  /** format - add support for PGM_P (PSTR) strings */
  int format_P(PGM_P str, ...);
  
  /** add support for PSTR and (const prog_char str[] PROGMEM) style strings. */
  size_t print_P(PGM_P str);
};

#endif