/*

  This file is a part of ErrUt, a small collection of error handling
  utilities.
  
  Copyright (c) 2008-2018 Jori Liesenborgs

  Contact: jori.liesenborgs@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

/**
 * \file booltype.h
 */

#ifndef ERRUT_BOOLTYPE_H

#define ERRUT_BOOLTYPE_H

#include "errutconfig.h"
#include <string.h>
#include <string>

namespace errut
{

#define ERRUT_BOOL_T_LEN											2048
#define ERRUT_BOOL_T_FAILMSG										"Unknown error"
#define ERRUT_BOOL_T_SUCCESSMSG										"Success"

/** 
 * Type to return true/false with error description.
 *
 * This type is intended to be used as a return value of functions, which
 * return true on success and false on failure. Additionally, when an error
 * is returned, an error description can be set. The error description can
 * be retrieved using the bool_t::getErrorString member function.
 *
 * To make it easier to use, you can just return an error description, in
 * which case the boolean value of the type will automatically be 'false'.
 */
class ERRUT_IMPORTEXPORT bool_t
{
public:
	/** Just set true or false, but leave the error description undefined in case of 'false'. */
	bool_t(bool f = true);

	/** Set the return value to 'false', and the error string to the specified value. */
	bool_t(const char *pStr);

	/** Set the return value to 'false', and the error string to the specified value. */
	bool_t(const std::string &err);

	/** Copy constructor. */
	bool_t(const bool_t &b);

	/** Assignment operator. */
	bool_t &operator=(const bool_t &b);

	/** Returns a description of the error. */
	std::string getErrorString() const;

	/** Returns true or false, depending on the contents of this object. */
	operator bool() const;

	/** Returns true or false, depending on the contents of this object. */
	bool success() const;
private:
	void strncpy(const char *pSrc);
	void setErrorString(const std::string &err);
	void setErrorString(const char *pStr);

	char m_errorString[ERRUT_BOOL_T_LEN];
};

inline bool_t::bool_t(bool f)
{
	if (f)
		m_errorString[0] = 0;
	else
		setErrorString(ERRUT_BOOL_T_FAILMSG);
}

inline bool_t::bool_t(const char *pStr)
{
	setErrorString(pStr);
}

inline bool_t::bool_t(const std::string &err)
{
	setErrorString(err);
}

inline void bool_t::strncpy(const char *pSrc)
{
#ifndef _WIN32
	::strncpy(m_errorString, pSrc, ERRUT_BOOL_T_LEN);
#else
	strncpy_s(m_errorString, ERRUT_BOOL_T_LEN, pSrc, _TRUNCATE);
#endif // !_WIN32
	m_errorString[ERRUT_BOOL_T_LEN-1] = 0;
}

inline bool_t::bool_t(const bool_t &b)
{
	if (b.m_errorString[0] == 0) // No error
		m_errorString[0] = 0;
	else
		strncpy(b.m_errorString);
}

inline void bool_t::setErrorString(const std::string &s)
{
	setErrorString(s.c_str());
}

inline void bool_t::setErrorString(const char *pStr)
{
	if (pStr == 0 || pStr[0] == 0)
		strncpy(ERRUT_BOOL_T_FAILMSG);
	else
		strncpy(pStr);
}

inline bool_t &bool_t::operator=(const bool_t &b)
{
	if (b.m_errorString[0] == 0) // No error
		m_errorString[0] = 0;
	else
		strncpy(b.m_errorString);

	return *this;
}

inline std::string bool_t::getErrorString() const
{
	if (m_errorString[0] == 0)
		return ERRUT_BOOL_T_SUCCESSMSG;
	return m_errorString;
}

inline bool_t::operator bool() const
{
	return success();
}

inline bool bool_t::success() const
{
	return (m_errorString[0] == 0);
}

} // end namespace

#endif // ERRUT_BOOLTYPE_H
