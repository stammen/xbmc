/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "Variant.h"

#include <stdlib.h>
#include <string>
#include <sstream>
#include <utility>
#include <algorithm>

#ifndef strtoll
#if defined(TARGET_WINDOWS) || defined(TARGET_WIN10)
#define strtoll  _strtoi64
#define strtoull _strtoui64
#define wcstoll  _wcstoi64
#define wcstoull _wcstoui64
#else // TARGET_WINDOWS
#if !defined(TARGET_DARWIN)
#define strtoll(str, endptr, base)  (int64_t)strtod(str, endptr)
#define strtoull(str, endptr, base) (uint64_t)strtod(str, endptr)
#define wcstoll(str, endptr, base)  (int64_t)wcstod(str, endptr)
#define wcstoull(str, endptr, base) (uint64_t)wcstod(str, endptr)
#endif
#endif // TARGET_WINDOWS
#endif // strtoll

std::string trimRight(const std::string &str)
{
  std::string tmp = str;
  // find_last_not_of will return string::npos (which is defined as -1)
  // or a value between 0 and size() - 1 => find_last_not_of() + 1 will
  // always result in a valid index between 0 and size()
  tmp.erase(tmp.find_last_not_of(" \n\r\t") + 1);

  return tmp;
}

std::wstring trimRight(const std::wstring &str)
{
  std::wstring tmp = str;
  // find_last_not_of will return string::npos (which is defined as -1)
  // or a value between 0 and size() - 1 => find_last_not_of() + 1 will
  // always result in a valid index between 0 and size()
  tmp.erase(tmp.find_last_not_of(L" \n\r\t") + 1);

  return tmp;
}

int64_t str2int64(const std::string &str, int64_t fallback /* = 0 */)
{
  char *end = NULL;
  std::string tmp = trimRight(str);
  int64_t result = strtoll(tmp.c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

int64_t str2int64(const std::wstring &str, int64_t fallback /* = 0 */)
{
  wchar_t *end = NULL;
  std::wstring tmp = trimRight(str);
  int64_t result = wcstoll(tmp.c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

uint64_t str2uint64(const std::string &str, uint64_t fallback /* = 0 */)
{
  char *end = NULL;
  std::string tmp = trimRight(str);
  uint64_t result = strtoull(tmp.c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

uint64_t str2uint64(const std::wstring &str, uint64_t fallback /* = 0 */)
{
  wchar_t *end = NULL;
  std::wstring tmp = trimRight(str);
  uint64_t result = wcstoull(tmp.c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

double str2double(const std::string &str, double fallback /* = 0.0 */)
{
  char *end = NULL;
  std::string tmp = trimRight(str);
  double result = strtod(tmp.c_str(), &end);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

double str2double(const std::wstring &str, double fallback /* = 0.0 */)
{
  wchar_t *end = NULL;
  std::wstring tmp = trimRight(str);
  double result = wcstod(tmp.c_str(), &end);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

CVariant::CVariant()
  : CVariant(VariantTypeNull)
{
}

CVariant CVariant::ConstNullVariant = CVariant::VariantTypeConstNull;

CVariant::CVariant(VariantType type)
{
  m_type = type;

  switch (type)
  {
    case VariantTypeInteger:
      m_integer = 0;
      break;
    case VariantTypeUnsignedInteger:
      m_unsignedinteger = 0;
      break;
    case VariantTypeBoolean:
      m_boolean = false;
      break;
    case VariantTypeDouble:
      m_dvalue = 0.0;
      break;
    case VariantTypeString:
      m_string.clear();
      break;
    case VariantTypeWideString:
      m_wstring.clear();
      break;
    case VariantTypeArray:
      m_array.clear();
      break;
    case VariantTypeObject:
      m_map.clear();
      break;
    default:
      //memset(&m_data, 0, sizeof(m_data));
      break;
  }
}

CVariant::CVariant(int integer)
{
  m_type = VariantTypeInteger;
  m_integer = integer;
}

CVariant::CVariant(int64_t integer)
{
  m_type = VariantTypeInteger;
  m_integer = integer;
}

CVariant::CVariant(unsigned int unsignedinteger)
{
  m_type = VariantTypeUnsignedInteger;
  m_unsignedinteger = unsignedinteger;
}

CVariant::CVariant(uint64_t unsignedinteger)
{
  m_type = VariantTypeUnsignedInteger;
  m_unsignedinteger = unsignedinteger;
}

CVariant::CVariant(double value)
{
  m_type = VariantTypeDouble;
  m_dvalue = value;
}

CVariant::CVariant(float value)
{
  m_type = VariantTypeDouble;
  m_dvalue = (double)value;
}

CVariant::CVariant(bool boolean)
{
  m_type = VariantTypeBoolean;
  m_boolean = boolean;
}

CVariant::CVariant(const char *str)
{
  m_type = VariantTypeString;
  m_string = str;
  m_map.clear();
}

CVariant::CVariant(const char *str, unsigned int length)
{
  m_type = VariantTypeString;
  m_string = std::string(str, length);
}

CVariant::CVariant(const std::string &str)
{
  m_type = VariantTypeString;
  m_string = str;
}

CVariant::CVariant(std::string &&str)
{
  m_type = VariantTypeString;
  m_string = std::move(str);
}

CVariant::CVariant(const wchar_t *str)
{
  m_type = VariantTypeWideString;
  m_wstring = str;
}

CVariant::CVariant(const wchar_t *str, unsigned int length)
{
  m_type = VariantTypeWideString;
  m_wstring = std::wstring(str, length);
}

CVariant::CVariant(const std::wstring &str)
{
  m_type = VariantTypeWideString;
  m_wstring = str;
}

CVariant::CVariant(std::wstring &&str)
{
  m_type = VariantTypeWideString;
  m_wstring = std::move(str);
}

CVariant::CVariant(const std::vector<std::string> &strArray)
{
  m_type = VariantTypeArray;
  m_array.clear();
  m_array.reserve(strArray.size());
  for (const auto& item : strArray)
    m_array.push_back(CVariant(item));
}

CVariant::CVariant(const std::map<std::string, std::string> &strMap)
{
  m_type = VariantTypeObject;
  m_map.clear();
  for (std::map<std::string, std::string>::const_iterator it = strMap.begin(); it != strMap.end(); ++it)
    m_map.insert(make_pair(it->first, it->second));
}

CVariant::CVariant(const std::map<std::string, CVariant> &variantMap)
{
  m_type = VariantTypeObject;
  for (auto it = variantMap.begin(); it != variantMap.end(); ++it)
    m_map.insert(make_pair(it->first, it->second));
}

#if 0
CVariant::CVariant(const CVariant &variant)
{
  m_type = VariantTypeNull;
  *this = variant;
}

CVariant::CVariant(CVariant&& rhs)
{
  //Set this so that operator= don't try and run cleanup
  //when we're not initialized.
  m_type = VariantTypeNull;

  *this = std::move(rhs);
}
#endif

bool CVariant::isInteger() const
{
  return isSignedInteger() || isUnsignedInteger();
}

bool CVariant::isSignedInteger() const
{
  return m_type == VariantTypeInteger;
}

bool CVariant::isUnsignedInteger() const
{
  return m_type == VariantTypeUnsignedInteger;
}

bool CVariant::isBoolean() const
{
  return m_type == VariantTypeBoolean;
}

bool CVariant::isDouble() const
{
  return m_type == VariantTypeDouble;
}

bool CVariant::isString() const
{
  return m_type == VariantTypeString;
}

bool CVariant::isWideString() const
{
  return m_type == VariantTypeWideString;
}

bool CVariant::isArray() const
{
  return m_type == VariantTypeArray;
}

bool CVariant::isObject() const
{
  return m_type == VariantTypeObject;
}

bool CVariant::isNull() const
{
  return m_type == VariantTypeNull || m_type == VariantTypeConstNull;
}

CVariant::VariantType CVariant::type() const
{
  return m_type;
}

int64_t CVariant::asInteger(int64_t fallback) const
{
  switch (m_type)
  {
    case VariantTypeInteger:
      return m_integer;
    case VariantTypeUnsignedInteger:
      return (int64_t)m_unsignedinteger;
    case VariantTypeDouble:
      return (int64_t)m_dvalue;
    case VariantTypeString:
      return str2int64(m_string, fallback);
    case VariantTypeWideString:
      return str2int64(m_wstring, fallback);
    default:
      return fallback;
  }
  
  return fallback;
}

uint64_t CVariant::asUnsignedInteger(uint64_t fallback) const
{
  switch (m_type)
  {
    case VariantTypeUnsignedInteger:
      return m_unsignedinteger;
    case VariantTypeInteger:
      return (uint64_t)m_integer;
    case VariantTypeDouble:
      return (uint64_t)m_dvalue;
    case VariantTypeString:
      return str2uint64(m_string, fallback);
    case VariantTypeWideString:
      return str2uint64(m_wstring, fallback);
    default:
      return fallback;
  }
  
  return fallback;
}

double CVariant::asDouble(double fallback) const
{
  switch (m_type)
  {
    case VariantTypeDouble:
      return m_dvalue;
    case VariantTypeInteger:
      return (double)m_integer;
    case VariantTypeUnsignedInteger:
      return (double)m_unsignedinteger;
    case VariantTypeString:
      return str2double(m_string, fallback);
    case VariantTypeWideString:
      return str2double(m_wstring, fallback);
    default:
      return fallback;
  }
  
  return fallback;
}

float CVariant::asFloat(float fallback) const
{
  switch (m_type)
  {
    case VariantTypeDouble:
      return (float)m_dvalue;
    case VariantTypeInteger:
      return (float)m_integer;
    case VariantTypeUnsignedInteger:
      return (float)m_unsignedinteger;
    case VariantTypeString:
      return (float)str2double(m_string, fallback);
    case VariantTypeWideString:
      return (float)str2double(m_wstring, fallback);
    default:
      return fallback;
  }
  
  return fallback;
}

bool CVariant::asBoolean(bool fallback) const
{
  switch (m_type)
  {
    case VariantTypeBoolean:
      return m_boolean;
    case VariantTypeInteger:
      return (m_integer != 0);
    case VariantTypeUnsignedInteger:
      return (m_unsignedinteger != 0);
    case VariantTypeDouble:
      return (m_dvalue != 0);
    case VariantTypeString:
      if (m_string.empty() || m_string.compare("0") == 0 || m_string.compare("false") == 0)
        return false;
      return true;
    case VariantTypeWideString:
      if (m_wstring.empty() || m_wstring.compare(L"0") == 0 || m_wstring.compare(L"false") == 0)
        return false;
      return true;
    default:
      return fallback;
  }
  
  return fallback;
}

std::string CVariant::asString(const std::string &fallback /* = "" */) const
{
  switch (m_type)
  {
    case VariantTypeString:
      return m_string;
    case VariantTypeBoolean:
      return m_boolean ? "true" : "false";
    case VariantTypeInteger:
    case VariantTypeUnsignedInteger:
    case VariantTypeDouble:
    {
      std::ostringstream strStream;
      if (m_type == VariantTypeInteger)
        strStream << m_integer;
      else if (m_type == VariantTypeUnsignedInteger)
        strStream << m_unsignedinteger;
      else
        strStream << m_dvalue;
      return strStream.str();
    }
    default:
      return fallback;
  }
  
  return fallback;
}

std::wstring CVariant::asWideString(const std::wstring &fallback /* = L"" */) const
{
  switch (m_type)
  {
    case VariantTypeWideString:
      return m_wstring;
    case VariantTypeBoolean:
      return m_boolean ? L"true" : L"false";
    case VariantTypeInteger:
    case VariantTypeUnsignedInteger:
    case VariantTypeDouble:
    {
      std::wostringstream strStream;
      if (m_type == VariantTypeInteger)
        strStream << m_integer;
      else if (m_type == VariantTypeUnsignedInteger)
        strStream << m_unsignedinteger;
      else
        strStream << m_dvalue;
      return strStream.str();
    }
    default:
      return fallback;
  }
  
  return fallback;
}

CVariant &CVariant::operator[](const std::string &key)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeObject;
    m_map.clear();
  }

  if (m_type == VariantTypeObject)
    return (m_map)[key];
  else
    return ConstNullVariant;
}

const CVariant &CVariant::operator[](const std::string &key) const
{
  VariantMap::const_iterator it;
  if (m_type == VariantTypeObject && (it = m_map.find(key)) != m_map.end())
    return it->second;
  else
    return ConstNullVariant;
}

CVariant &CVariant::operator[](unsigned int position)
{
  if (m_type == VariantTypeArray && size() > position)
    return m_array.at(position);
  else
    return ConstNullVariant;
}

const CVariant &CVariant::operator[](unsigned int position) const
{
  if (m_type == VariantTypeArray && size() > position)
    return m_array.at(position);
  else
    return ConstNullVariant;
}

CVariant &CVariant::operator=(const CVariant &rhs)
{
  if (m_type == VariantTypeConstNull || this == &rhs)
    return *this;

  m_type = rhs.m_type;

  switch (m_type)
  {
  case VariantTypeInteger:
    m_integer = rhs.m_integer;
    break;
  case VariantTypeUnsignedInteger:
    m_unsignedinteger = rhs.m_unsignedinteger;
    break;
  case VariantTypeBoolean:
    m_boolean = rhs.m_boolean;
    break;
  case VariantTypeDouble:
    m_dvalue = rhs.m_dvalue;
    break;
  case VariantTypeString:
    m_string = rhs.m_string;
    break;
  case VariantTypeWideString:
    m_wstring = rhs.m_wstring;
    break;
  case VariantTypeArray:
    m_array = VariantArray(rhs.m_array.begin(), rhs.m_array.end());
    break;
  case VariantTypeObject:
    m_map = VariantMap(rhs.m_map.begin(), rhs.m_map.end());
    break;
  default:
    break;
  }

  return *this;
}


bool CVariant::operator==(const CVariant &rhs) const
{
  if (m_type == rhs.m_type)
  {
    switch (m_type)
    {
    case VariantTypeInteger:
      return m_integer == rhs.m_integer;
    case VariantTypeUnsignedInteger:
      return m_unsignedinteger == rhs.m_unsignedinteger;
    case VariantTypeBoolean:
      return m_boolean == rhs.m_boolean;
    case VariantTypeDouble:
      return m_dvalue == rhs.m_dvalue;
    case VariantTypeString:
      return m_string == rhs.m_string;
    case VariantTypeWideString:
      return m_wstring == rhs.m_wstring;
    case VariantTypeArray:
      return m_array.size() == rhs.m_array.size() && std::equal(m_array.begin(), m_array.end(), rhs.m_array.begin());
    case VariantTypeObject:
      return m_map.size() == rhs.m_map.size() && std::equal(m_map.begin(), m_map.end(), rhs.m_map.begin());
    default:
      break;
    }
  }

  return false;
}

void CVariant::push_back(const CVariant &variant)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeArray;
    m_array.clear();
  }

  if (m_type == VariantTypeArray)
    m_array.push_back(variant);
}

void CVariant::push_back(CVariant &&variant)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeArray;
    m_array.clear();
  }

  if (m_type == VariantTypeArray)
    m_array.push_back(std::move(variant));
}

void CVariant::append(const CVariant &variant)
{
  push_back(variant);
}

void CVariant::append(CVariant&& variant)
{
  push_back(std::move(variant));
}

const char *CVariant::c_str() const
{
  if (m_type == VariantTypeString)
    return m_string.c_str();
  else
    return NULL;
}


CVariant::iterator_array CVariant::begin_array()
{
  if (m_type == VariantTypeArray)
    return m_array.begin();
  else
    return iterator_array();
}

CVariant::const_iterator_array CVariant::begin_array() const
{
  if (m_type == VariantTypeArray)
    return m_array.begin();
  else
    return const_iterator_array();
}

CVariant::iterator_array CVariant::end_array()
{
  if (m_type == VariantTypeArray)
    return m_array.end();
  else
    return iterator_array();
}

CVariant::const_iterator_array CVariant::end_array() const
{
  if (m_type == VariantTypeArray)
    return m_array.end();
  else
    return const_iterator_array();
}

CVariant::iterator_map CVariant::begin_map()
{
  if (m_type == VariantTypeObject)
    return m_map.begin();
  else
    return iterator_map();
}

CVariant::const_iterator_map CVariant::begin_map() const
{
  if (m_type == VariantTypeObject)
    return m_map.begin();
  else
    return const_iterator_map();
}

CVariant::iterator_map CVariant::end_map()
{
  if (m_type == VariantTypeObject)
    return m_map.end();
  else
    return iterator_map();
}

CVariant::const_iterator_map CVariant::end_map() const
{
  if (m_type == VariantTypeObject)
    return m_map.end();
  else
    return const_iterator_map();
}

unsigned int CVariant::size() const
{
  if (m_type == VariantTypeObject)
    return m_map.size();
  else if (m_type == VariantTypeArray)
    return m_array.size();
  else if (m_type == VariantTypeString)
    return m_string.size();
  else if (m_type == VariantTypeWideString)
    return m_wstring.size();
  else
    return 0;
}

bool CVariant::empty() const
{
  if (m_type == VariantTypeObject)
    return m_map.empty();
  else if (m_type == VariantTypeArray)
    return m_array.empty();
  else if (m_type == VariantTypeString)
    return m_string.empty();
  else if (m_type == VariantTypeWideString)
    return m_wstring.empty();
  else if (m_type == VariantTypeNull)
    return true;

  return false;
}

void CVariant::clear()
{
  if (m_type == VariantTypeObject)
    m_map.clear();
  else if (m_type == VariantTypeArray)
    m_array.clear();
  else if (m_type == VariantTypeString)
    m_string.clear();
  else if (m_type == VariantTypeWideString)
    m_wstring.clear();
}

void CVariant::erase(const std::string &key)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeObject;
    m_map.clear();
  }
  else if (m_type == VariantTypeObject)
    m_map.erase(key);
}

void CVariant::erase(unsigned int position)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeArray;
    m_array.clear();
  }

  if (m_type == VariantTypeArray && position < size())
    m_array.erase(m_array.begin() + position);
}

bool CVariant::isMember(const std::string &key) const
{
  if (m_type == VariantTypeObject)
    return m_map.find(key) != m_map.end();

  return false;
}
