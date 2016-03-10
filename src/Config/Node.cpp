/*
    Copyright (C) 2016, EdWard <ezeward4@gmail.com>
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

// Project Includes
#include "Node.h"

// Macro
#define STRING_ENUM(x) [x] = #x
#define SE(x) STRING_ENUM(x)

namespace dman
{

const char * const Node::schema_formats[] = {
	SE(color),
	SE(date),
	SE(datetime),
	SE(datetime_local),
	SE(email),
	SE(month),
	SE(number),
	SE(range),
	SE(tel),
	SE(text),
	SE(textarea),
	SE(time),
	SE(url),
	SE(week),
	SE(checkbox),
	SE(table),
	SE(tabs),
	SE(select),
	SE(grid)
};

const char * const Node::get_format_string(SchemaFormat_t format)
{
	return schema_formats[format];
}

std::string Node::GetDescription() const
{
	if (base_schema_.count("description"))
		return base_schema_.at("description");
	return "";
}

void Node::SetDescription(std::string description)
{
	base_schema_["description"] = std::move(description);
}

std::string Node::GetFormat() const
{
	if (base_schema_.count("format"))
		return base_schema_.at("format");
	return "";
}

void Node::SetFormat(std::string format)
{
	base_schema_["format"] = std::move(format);
}

void Node::SetFormat(SchemaFormat_t format)
{
	SetFormat(get_format_string(format));
}

bool Node::IsFormat(SchemaFormat_t format) const
{
	return GetFormat() == get_format_string(format);
}

}  // namespace dman
