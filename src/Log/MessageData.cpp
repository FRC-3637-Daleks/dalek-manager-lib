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


// Project Includes
#include "MessageData.h"

// STD Includes
#include <string>
#include <sstream>
#include <iostream>

namespace dman
{

std::ostream& operator<<(std::ostream& strm, const MessageData &data)
{
	// Enum to string array only visible at function scope
	static const char * message_strings[] = {
		[MessageData::STATUS] = "STATUS",
		[MessageData::INFO]   = "INFO",
		[MessageData::WARN]   = "WARN",
		[MessageData::ERROR]  = "ERROR",
		[MessageData::FATAL]  = "FATAL"
	};

	strm << data.get_message_type() << ":" << data.get_verbosity();

	return strm;
}

const std::string MessageData::ToString() const
{
	std::ostringstream ret;
	ret << *this;
	return ret.str();
}

const uint16_t MessageData::GetSeverity() const
{
	return get_message_type() << 8 | ~get_verbosity();
}

}  // namespace dman
