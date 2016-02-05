/*
	Copyright (C) 2015 EdWard <ezeward4@gmail.com>
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

#ifndef SRC_LOG_LOGATTRIBUTES_H_
#define SRC_LOG_LOGATTRIBUTES_H_

// Project Includes
#include "MessageData.h"
#include "SystemData.h"


// Boost Includes
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/expressions/keyword_fwd.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/expressions.hpp>

// STD Includes
#include <iomanip>

namespace dman
{

// Boost log attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(message_data, "Severity", MessageData)
BOOST_LOG_ATTRIBUTE_KEYWORD(system_data, "Channel", SystemData)

/// Default format used by sinks
const auto default_log_format =
		boost::log::expressions::stream <<
		std::setw(8) << std::setfill('0') << line_id.or_none() <<
		": [" << system_data.or_throw() << "] <" <<
		message_data.or_throw() << "> " <<
		boost::log::expressions::message;

/// Custom Logger alias for convenience
using text_logger =
    boost::log::sources::severity_channel_logger<MessageData, SystemData>;

/// Custom Mutex Logger alias for convenience
using text_logger_mt =
    boost::log::sources::severity_channel_logger_mt<MessageData, SystemData>;

}  // namespace dman

#endif  // SRC_LOG_LOGATTRIBUTES_H_
