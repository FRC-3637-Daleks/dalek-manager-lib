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

// Project Inludes
#include "Setting.h"
#include "SettingTypeMismatchError.h"

namespace dman
{

const char * const Setting::JSONTypeToString(json::value_t type)
{
    switch (type)
    {
    case json::value_t::object:
        return "object";
    case json::value_t::array:
        return "array";
    case json::value_t::string:
        return "string";
    case json::value_t::boolean:
        return "boolean";
    case json::value_t::number_integer:
    // case json::value_t::number_unsigned:
        return "integer";
    case json::value_t::number_float:
        return "number";
    default:
        return "null";
    }
}

bool Setting::CanConvert(json::value_t from, json::value_t to)
{
	using value_t = json::value_t;

	if(from == to)
		return true;

	if(to == value_t::null)
		return true;

	if(from == value_t::number_integer && to == value_t::number_float ||
		from == value_t::number_float && to == value_t::number_integer)
		return true;

	return false;
}

Setting::Setting(json::value_t type): type_(type)
{
}

json Setting::GetValueOrDefault()
{
    if (is_empty())
        return get_default();
    else
        return get_value();
}

const json& Setting::GetValueOrDefault() const
{
    if (is_empty())
        return get_default();
    else
        return get_value();
}

void Setting::SetValue(json value)
{
    if (has_no_type() ||
		get_type() == value.type() ||
		value.is_number() && value_.is_number())
    {
        type_ = value.type();
        value_ = std::move(value);
    }
    else
        throw SettingTypeMismatchError(get_type(), value.type());
}

void Setting::SetDefault(json default_value)
{
    if (has_no_type() ||
		get_type() == default_value.type() ||
		default_value.is_number() && value_.is_number())
    {
        type_ = default_value.type();
        default_value_ = std::move(default_value);
    }
    else
        throw SettingTypeMismatchError(get_type(),
            default_value.type());
}

bool Setting::LoadConfig(const json& config)
{
    if (has_no_type() ||
		get_type() == config.type() ||
		config.is_number() && value_.is_number())
	{
        SetValue(config);
	}
    else
    {
        SetValue(default_value_);
        return true;
    }

    return false;
}

json Setting::GetConfig() const
{
    return value_;
}

json Setting::GetSchema() const
{
    json ret(base_schema_);

    ret["type"] = JSONTypeToString(get_type());
    ret["default"] = get_default();

    return ret;
}

}  // namespace dman
