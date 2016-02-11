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

#include "plist_decoder.hpp"
#include "value.hpp"

#include <exception>

pbx::PlistDecoder::PlistDecoder(const std::string& str) :
_index(0),
_str(str)
{}

void pbx::PlistDecoder::_ignore_comment()
{
	if (_index + 1 >= _str.length() || (_str[_index] != '/' || _str[_index + 1] != '*'))
		return;
	
	_index += 2;
	
	while (_str[_index] != '*' && _str[_index + 1] != '/')
		_index++;
	
	_index += 2;
}

void pbx::PlistDecoder::_ignore_whitespaces()
{
	while (_index < _str.length() && _is_whitespace(_str[_index]))
		_index++;
}

bool pbx::PlistDecoder::_is_whitespace(const char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

pbx::Dictionary pbx::PlistDecoder::parse(const std::string& str)
{
	pbx::PlistDecoder decoder(str);
	return decoder._parse();
}

pbx::Dictionary pbx::PlistDecoder::_parse()
{
	if (_str[0] == '/' && _str[1] == '/')
		while (_str[_index] != '{')
			_index++;
	
	Dictionary result = _parse_dictionary();
	return result;
}

pbx::Array pbx::PlistDecoder::_parse_array()
{
	pbx::Array obj;
	
	if (_str[_index] != '(')
		throw std::invalid_argument("Arrays are expected to start with the '(' character.");
	
	_index++;
	_parse_padding();
	
	while (_str[_index] != ')')
	{
		_parse_array_entry(obj);
		_parse_padding();
	}
	
	_index++;
	_parse_padding();
	
	return obj;
}

void pbx::PlistDecoder::_parse_array_entry(pbx::Array &arr)
{
	pbx::Value value = _parse_value();
	
	if (_str[_index] != ',')
		throw std::invalid_argument("Expects the ',' character after a value.");
	_index++;
	
	arr.push_back(value);
}

pbx::Dictionary pbx::PlistDecoder::_parse_dictionary()
{
	Dictionary obj;
	
	if (_str[_index] != '{')
		throw std::invalid_argument("Dictionaries are expected to start with the '{' character.");
	
	_index++;
	_parse_padding();
	
	while (_str[_index] != '}')
	{
		_parse_dictionary_entry(obj);
		_parse_padding();
	}
	
	_index++;
	_parse_padding();
	
	return obj;
}

void pbx::PlistDecoder::_parse_dictionary_entry(pbx::Dictionary& dict)
{
	std::string key = _parse_key();
	
	if (_str[_index] != '=')
		throw std::invalid_argument("Expects the '=' character after the key of a dictionary entry.");
	
	_index++;
	_parse_padding();
	pbx::Value value = _parse_value();
	
	if (_str[_index] != ';')
		throw std::invalid_argument("Expects the ';' character after the value of a dictionary entry.");
	
	dict[key] = value;
	_index++;
}

std::string pbx::PlistDecoder::_parse_key()
{
	_parse_padding();
	
	std::string key;
	while (_index < _str.length() && !_is_whitespace(_str[_index]) && _str[_index] != ';')
		key += _str[_index++];
	
	_parse_padding();
	if (key[0] == '"') key = key.replace(0, 1, "");
	if (key[key.length()-1] == '"') key = key.replace(key.length()-1, key.length(), "");
	
	return key;
}


std::string pbx::PlistDecoder::_parse_literal()
{
	_parse_padding();
	std::string literal;
	
	if (_str[_index] == '"')
	{
		_index++;
		while (_index < _str.length() && (_str[_index] != '"' ||  (_str[_index] == '"' && _str[_index - 1] == '\\')))
			literal += _str[_index++];
		_index++;
	} else
		while (_index < _str.length() && !_is_whitespace(_str[_index]) && _str[_index] != ';' && _str[_index] != ',')
			literal += _str[_index++];
	
	_parse_padding();
	return literal;
}

void pbx::PlistDecoder::_parse_padding()
{
	_ignore_whitespaces();
	_ignore_comment();
	_ignore_whitespaces();
}

pbx::Value pbx::PlistDecoder::_parse_value()
{
	switch (_str[_index])
	{
		case '{':
			return _parse_dictionary();
		case '(':
			return _parse_array();
		default:
			return _parse_literal();
	}
}
