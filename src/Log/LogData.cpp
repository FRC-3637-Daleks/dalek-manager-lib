/*
    Copyright (C) 2015, EdWard <ezeward4@gmail.com>
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

#include <string>
#include <sstream>
#include "LogData.h"

namespace dman
{

const std::string LogData::MessageData::ToString() const
{
	// Enum to string array only visible at function scope
	static const char * message_strings[] = {
		[STATUS] = "STATUS",
		[INFO]   = "INFO",
		[WARN]   = "WARN",
		[ERROR]  = "ERROR"
	};

	// Uses a stringstream to form the string representation
	std::ostringstream ret(message_strings[message_type_]);

	ret << ":" << verbosity_;

	return ret.str();
}

const std::string LogData::SystemData::ToString() const
{
	return system_+" \""+component_name_+"\":"+component_type_;
}

const std::string LogData::ToString() const
{
	std::ostringstream ret;
	ret << timestamp_.count() << ": <" << message_data_.ToString() << ">";
	ret << " [" << system_data_.ToString() << "]: " << message_;

	return ret.str();
}

}  // namespace dman
