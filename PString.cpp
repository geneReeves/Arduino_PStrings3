/*
  PString.cpp - Lightweight printable string class
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

#include "PString.h"
/**
 * \file PString.cpp
 * \brief Implementation for PString class
 * \remark Update - GgR 04-15-2012 - added code to begin() to zero entire buffer.
 * \remark Update - GgR 04-15-2012 - added format(__FlashStringHelper *str, ..);
 * \remark Update - GgR 04-15-2012 - added format_P(PGM_P str, ...);
 * \remark Update - GgR 04-15-2012 - added print_P(PGM_P str, ...);
 * \remark Update - GgR 04-15-2012 - changed setBuffer buffer==NULL && size > 0 = PString will allocate buffer and free it on destroy.
 * \remark Update - GgR 04-15-2012 - changed Constructor to allow for empty constructor
 * \remark Update - GgR 04-15-2012 - added setBuffer(char *buffer, size_t size);
 * \remark Update - GgR 04-16-2012 - fised bug in setBuffer (did not free buffer if we owned it)
 */
void PString::begin()
{
  _cur = _buf;
  if (!_buf)
  {
	_size = 0;
  }
  if (_size > 0)
  {
	// zero entire buffer !
    /* _buf[0] = '\0'; */
	memset(_buf, 0 , _size);
  }
}

void PString::setBuffer(char *buffer, size_t size) 
{
  if ((_weOwn) &&(_buf))
	free(_buf);
  
  _buf=buffer; 
  _size=size; 
  if ((_buf==NULL) && (_size > 0))
  {
	// allocating our own buffer;
	_weOwn = true;
	_buf = (char *)calloc(_size + 1, sizeof(char));
  }
  begin();
} 

#if defined(ARDUINO) && ARDUINO >= 100
size_t PString::write(uint8_t b)
#else
void PString::write(uint8_t b)
#endif
{
  if (_cur + 1 < _buf + _size)
  {
    *_cur++ = (char)b;
    *_cur = '\0';
#if defined(ARDUINO) && ARDUINO >= 100
		return 1;
#endif
	}

#if defined(ARDUINO) && ARDUINO >= 100
	return 0;
#endif
}

int PString::format(char *str, ...) 
{ 
  va_list argptr;  
  va_start(argptr, str); 
  int ret = vsnprintf(_cur, _size - (_cur - _buf), str, argptr);
  if (_size)
     while (*_cur) 
        ++_cur;
  return ret;
}

int PString::format(const __FlashStringHelper *str, ...) 
{ 
  va_list argptr;  
  va_start(argptr, str); 
  const prog_char *p = (const prog_char *)str;
  int ret = vsnprintf_P((char *)_cur, _size - (_cur - _buf), p, argptr);
  va_end(argptr);
  if (_size)
     while (*_cur) 
        ++_cur;
  return ret;
}

int PString::format_P(PGM_P str, ...) 
{ 
  va_list argptr;  
  va_start(argptr, str); 
  int ret = vsnprintf_P((char *)_cur, _size - (_cur - _buf), str, argptr);
  va_end(argptr);
  if (_size)
     while (*_cur) 
        ++_cur;
  return ret;
}

size_t PString::print_P(PGM_P str)
{
  while (_buf[*_cur++] = pgm_read_byte(str++)) 
    if (*_cur == _size)
	  break;
  _buf[*_cur++] ='\0';
}



