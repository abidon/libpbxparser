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

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace pbx {
	class Value;
	
	typedef std::vector<Value> Array;
	typedef std::map<std::string, Value> Dictionary;
	
	class Value
	{
	public:
		typedef enum
		{
			UnknownType,
			ArrayType,
			DictionaryType,
			StringType,
		} Type;
		
	private:
		Type _type;
		
		pbx::Array _a;
		pbx::Dictionary _d;
		std::string _s;
		
	public:
		Value() = default;
		Value(const pbx::Array& a);
		Value(const pbx::Dictionary& d);
		Value(const std::string& s);
		Value(const pbx::Value& copy) = default;
		
		pbx::Array&
		array_value();
		
		const pbx::Array&
		array_value() const;
		
		pbx::Dictionary&
		dictionary_value();
		
		const pbx::Dictionary&
		dictionary_value() const;
		
		void
		reset();
		
		std::string&
		string_value();
		
		const std::string&
		string_value() const;
		
		const pbx::Value::Type
		type() const;
		
		const uint64_t
		unsigned_value() const;
		
		pbx::Value& operator=(const pbx::Array& a);
		pbx::Value& operator=(const pbx::Dictionary& d);
		pbx::Value& operator=(const std::string& s);
		pbx::Value& operator=(const pbx::Value& copy) = default;
	};
}