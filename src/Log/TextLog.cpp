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
#include "TextLog.h"
#include "LogData.h"

// Boost Includes
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/core/null_deleter.hpp>

// STD Includes
#include <string>
#include <utility>
#include <stdexcept>
#include <iostream>

namespace dman
{

TextLog::CorePtr TextLog::core_(nullptr);

TextLog::Core& TextLog::GetCore()
{
	/// Returns core if already initialized
	if(core_)
		return *core_;

	/// Initializes core otherwise
	Initialize();

	/// If core still hasn't initialized it will still be null, this is bad
	if(!core_)
		throw std::runtime_error("Boost log core failed to initialize");

	/// Returns core by reference to guarantee not null
	return *core_;
}

void TextLog::Initialize()
{
	using namespace boost::log;

	typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
	boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

	/// Create standard output stream log
	sink->locked_backend()->add_stream(
		boost::shared_ptr< std::ostream >(&std::clog, boost::null_deleter()));

	/// Set internal core
	core_ = core::get();

	GetCore().add_sink(sink);
}

void TextLog::Log(MessageData&& message_data,
						 SystemData&& system_data,
						 std::string&& message)
{

}

void TextLog::Log(MessageData&& message_data, std::string&& message)
{
	Log(std::move(message_data),
		SystemData("Logging", "Logger", "TextLog"),
		std::move(message));
}

}  // namespace dman
