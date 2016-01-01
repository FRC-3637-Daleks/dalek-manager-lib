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

#ifndef SRC_LOG_TEXTLOG_H_
#define SRC_LOG_TEXTLOG_H_

// Project Includes
#include "StreamHandle.h"

// Boost Includes
#include <boost/log/core/core.hpp>

// STD Includes
#include <string>
#include <functional>

namespace dman
{

// Forward declarations
class MessageData;
class SystemData;


/** Front end for boost logging system.
 * TextLog manages the logging core and is the interface for logging specific to 
 * the rest of the library. It maintains sinks, sources, etc.
 */
class TextLog
{
public:
	/// Log Sink type
	using Sink = boost::log::sinks::sink;

	/// Log Sink Pointer Type
	using SinkPtr = boost::shared_ptr<Sink>;

	/// SimpleSink functor type
	using SimpleSinkFn = std::function<void(std::string)>;

	/** Forwards boost log call
	 * @param mess_data Message Type and Verbosity information
	 * @param sys_data System, Component, and Component type information
	 * @param message The log message
	 */
	static void Log(const MessageData &mess_data,
					SystemData sys_data,
					std::string message);

	/** Returns StremHandle set to forward boost log call on destruction
	 * @param mess_data Message Type and Verbosity information
	 * @param sys_data System, Component, and Component type information
	 * @return A StreamHandle object which can be used like a std stream and
	 * flushes on destruction
	 */
	static StreamHandle Log(const MessageData &mess_data, SystemData sys_data);

	/// Adds Sink to core
	static void AddSink(const SinkPtr& sink);

	/** Adds Simple Sink to core, a functor which will put a string somewhere
	 * @param sink_fn a function object supporing operator(std::string)
	 */
	static void AddSimpleSink(SimpleSinkFn sink_fn);

private:
	/// Log Core type
	using Core = boost::log::core;

	/// Log Core Pointer Type
	using CorePtr = boost::log::core_ptr;

	/// Reference to boost's log core
	static CorePtr core_;

	/** Returns a reference to the core, initializing it if needed.
	 * @exception std::runtime_error The boost log core returned a null core.
	 */
	static Core& GetCore();

	/// Configures logging core for use in this library and sets core.
	static void Initialize();

	/// Logs for the logging core itself
	static void Log(const MessageData &mess_data, std::string message);

	/// StreamHandle interface for logging of logging core itself
	static StreamHandle Log(const MessageData &mess_data);
};

}  // namespace dman

#endif  // SRC_LOG_TEXTLOG_H_
