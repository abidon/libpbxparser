/*
 * libpbxparser
 * Copyright (c) 2016 Aurelien Bidon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "value.hpp"

#include <sstream>

#define VALUE_TYPE_AS_STRING(t) (t == pbx::Value::StringType ? "string" : (t == pbx::Value::ArrayType ? "array" : (t == pbx::Value::DictionaryType ? "dictionary" : "unknown")))

pbx::Value::Value(const pbx::Array& a) :
_a(a),
_type(pbx::Value::ArrayType)
{}

pbx::Value::Value(const pbx::Dictionary& d) :
_d(d),
_type(pbx::Value::DictionaryType)
{}

pbx::Value::Value(const std::string& s) :
_s(s),
_type(pbx::Value::StringType)
{}

pbx::Array&
pbx::Value::array_value()
{
	if (type() != pbx::Value::ArrayType)
		throw std::invalid_argument(std::string("Can't return value as an array when it's type is ") + VALUE_TYPE_AS_STRING(type()));
	
	return _a;
}

const pbx::Array&
pbx::Value::array_value() const
{
	if (type() != pbx::Value::ArrayType)
		throw std::invalid_argument(std::string("Can't return value as an array when it's type is ") + VALUE_TYPE_AS_STRING(type()));
	
	return _a;
}

pbx::Dictionary&
pbx::Value::dictionary_value()
{
	if (type() != pbx::Value::DictionaryType)
		throw std::invalid_argument(std::string("Can't return value as a dictionary when it's type is ") + VALUE_TYPE_AS_STRING(type()));
	
	return _d;
}

const pbx::Dictionary&
pbx::Value::dictionary_value() const
{
	if (type() != pbx::Value::DictionaryType)
		throw std::invalid_argument(std::string("Can't return value as a dictionary when it's type is ") + VALUE_TYPE_AS_STRING(type()));
	
	return _d;
}

void
pbx::Value::reset()
{
	_a.clear();
	_d.clear();
	_s.clear();
	_type = pbx::Value::UnknownType;
}

std::string&
pbx::Value::string_value()
{
	if (type() != pbx::Value::StringType)
		throw std::invalid_argument(std::string("Can't return value as a string when it's type is ") + VALUE_TYPE_AS_STRING(type()));
	
	return _s;
}

const std::string&
pbx::Value::string_value() const
{
	if (type() != pbx::Value::StringType)
		throw std::invalid_argument(std::string("Can't return value as a string when it's type is ") + VALUE_TYPE_AS_STRING(type()));
	
	return _s;
}

const pbx::Value::Type
pbx::Value::type() const
{
	return _type;
}

const uint64_t
pbx::Value::unsigned_value() const
{
	if (type() != pbx::Value::StringType)
		throw std::invalid_argument(std::string("Can't return value as a string when it's type is ") + VALUE_TYPE_AS_STRING(type()));
	
	uint64_t num;
	
	std::stringstream ss(_s);
	ss >> num;
	
	return num;
}

pbx::Value& pbx::Value::operator=(const pbx::Array& a)
{
	reset();
	
	_a = a;
	_type = pbx::Value::ArrayType;
	
	return *this;
}

pbx::Value& pbx::Value::operator=(const pbx::Dictionary& d)
{
	reset();
	
	_d = d;
	_type = pbx::Value::DictionaryType;
	
	return *this;
}

pbx::Value& pbx::Value::operator=(const std::string& s)
{
	reset();
	
	_s = s;
	_type = pbx::Value::StringType;
	
	return *this;
}
