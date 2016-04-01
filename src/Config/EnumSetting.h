/*
    Copyright (C) 2016 EdWard <ezeward4@gmail.com>
    ---
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA  02110-1301, USA.

 */

#ifndef SRC_CONFIG_ENUMSETTING_H_
#define SRC_CONFIG_ENUMSETTING_H_

// Project Includes
#include "SettingWrapper.h"

namespace dman
{

/** Creates a setting that has a checkbox of an enumeration set
 * Converts the string value to the corresponding enum value
 * @tparam EnumT Must be an enumeration with elements from 0 to EnumT \\
 * e.g. \code{.cpp} enum Enum {state1 = 0, state2, N} \endcode
 * @tparam EnumN Number of values in the enum. By default looks for \\
 * EnumT::N. If this value is not present in the enum, this param must be set
 */
template<class EnumT, int EnumN = EnumT::N>
class EnumWrapper: SettingWrapper<std::string>
{
public:
	using Base_t = SettingWrapper<std::string>;
	using Base_t::Base_t;
	using Base_t::operator=;

public:
	/// Array containing the string representations of the enum values
	static std::string string_reps[EnumN];

	static void SetString(EnumT val, std::string str_rep)
	{
		int vall = static_cast<int>(val);
		if (vall < 0 || vall >= EnumN)
			return;
		else
			string_reps[vall] = std::move(str_rep);
	}

	static std::string GetString(EnumT val)
	{
		int vall = static_cast<int>(val);
		if (vall < 0 || vall >= EnumN)
			return "";
		return string_reps[vall];
	}

	static EnumT GetEnum(const std::string& rep)
	{
		for (int i = 0; i < EnumN; i++)
		{
			if (string_reps[i] == rep)
				return static_cast<EnumT>(i);
		}

		return static_cast<EnumT>(EnumN);
	}

public:
	explicit EnumWrapper(Reference_t ref): Base_t(ref)
	{
		if (ref)
		{
			ref->SetFormat(Node::SchemaFormat_t::select);
			json::array_t enums;
			for (auto str_rep : string_reps)
				enums.push_back(str_rep);
			ref->base_schema_["enum"] = enums;
		}
	}

public:
	EnumT GetEnumValue() const {return GetEnum(GetValue());}
	EnumT GetEnumDefault() const {return GetEnum(GetValue());}
	EnumT GetEnumValueOrDefault() const {return GetEnum(GetValueOrDefault());}

	void SetEnumValue(EnumT val) {SetValue(GetString(val));}
	void SetEnumDefault(EnumT val) {SetDefault(GetString(val));}
};

template<class EnumT, int EnumN>
std::string EnumWrapper<EnumT, EnumN>::string_reps[] = {""};

}  // namespace dman

#endif  // SRC_CONFIG_ENUMSETTING_H_
