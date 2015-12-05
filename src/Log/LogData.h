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

#ifndef SRC_LOG_LOGDATA_H_
#define SRC_LOG_LOGDATA_H_


// Boost Includes
#include <boost/log/sources/severity_channel_logger.hpp>

// STD Includes
#include <string>
#include <chrono>

namespace dman
{

using std::string;


/** Object which contains meta data on a per log basis.
 * Information contains type and importance of message.
 */
class MessageData
{
public:
	/// Specifies what kind of information is in the message
	enum Message_t {
		STATUS = 0,		///< Message is indicating a state change within the system
		INFO,			///< Message is providing information about the system
		WARN,			///< Message is warning user of potential conflict
		ERROR,			///< Message is indicating a runtime error of some kind
		FATAL			///< Message is indicating that the error should force program abort
	};

	/** Represents how important the information is. 
	 * The greater the value the less important it is.
	 */
	typedef uint8_t Verbosity_t;

public:
	constexpr MessageData(const Message_t m, const Verbosity_t v = 0):
		message_type_(m), verbosity_(v) {}
	constexpr MessageData(const MessageData& other) = default;
	constexpr MessageData(MessageData&& other) = default;

public:
	constexpr const Message_t get_message_type() const {return message_type_;}
	constexpr const Verbosity_t get_verbosity() const {return verbosity_;}

	/** Returns a string representation of the object used in the log text
	 * Formatted as MessageType:Verbosity
	 */
	const string ToString() const;

	/** Returns a comparable value representing how severe the log is.
	 * The high word is the Messsage Type, the low word is the inverse Verbosity
	 */
	const uint16_t GetSeverity() const;

public:
	/// Implicit conversion to string
	operator const string() const {return ToString();}

	/// Implicit conversion to uint16_t
	operator const uint16_t() const {return GetSeverity();}

private:
	Message_t message_type_;   ///< Type of the message
	Verbosity_t verbosity_;    ///< Verbosity of the message
};



/** Object which contains meta data for a component a log is describing
 * Information contains hierarchy: SystemName "ComponentName":ComponentType
 */
class SystemData
{
public:
	SystemData(const string& system,
						const string& component_name,
						const string& component_type):
						system_(system),
						component_name_(component_name),
						component_type_(component_type) {}
	SystemData(const SystemData& other) = default;
	SystemData(SystemData&& other) = default;

public:
	const string get_system() const {return system_;}
	const string get_component_name() const {return component_name_;}
	const string get_component_type() const {return component_type_;}

	/// Returns a string representation of the object used in the log text
	const string ToString() const;

public:
	/// Implicit conversion to string
	operator const string() const {return ToString();}

private:
	/** Subsystem path
 	 * e.g. "Lifter/Claw", "Cobra", "Drive"
 	 */
	string system_;

	/** Name of the componennt being described relative to the subsystem.
	 * e.g. "Right Front Motor", "Left Piston", etc.
	 */
	string component_name_;

	/** Type of the component being described
	 * e.g. "TalonSRX", "Solenoid-in"
	 */
	string component_type_;
};

// Custom Logger typedefs for convenience
using text_logger =
    boost::log::sources::severity_channel_logger<MessageData, SystemData>;

using text_logger_mt =
    boost::log::sources::severity_channel_logger_mt<MessageData, SystemData>;

}  // namespace dman

#endif  // SRC_LOG_LOGDATA_H_
