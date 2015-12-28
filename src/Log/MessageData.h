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

#ifndef SRC_LOG_MESSAGEDATA_H_
#define SRC_LOG_MESSAGEDATA_H_

// STD Includes
#include <string>
#include <iostream>

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
	enum {
		STATUS = 0,		///< Message is indicating a state change within the system
		INFO,			///< Message is providing information about the system
		WARN,			///< Message is warning user of potential conflict
		ERROR,			///< Message is indicating a runtime error of some kind
		FATAL			///< Message is indicating that the error should force program abort
	};
	using Message_t = uint8_t;

	/** Represents how important the information is. 
	 * The greater the value the less important it is.
	 */
	using Verbosity_t = uint8_t;

public:
	constexpr MessageData(const Message_t m, const Verbosity_t v = 0):
		message_type_(m), verbosity_(v) {}
	constexpr MessageData(const MessageData& other) = default;
	MessageData& operator= (const MessageData& other) = default;

public:
	constexpr Message_t get_message_type() const {return message_type_;}
	constexpr Verbosity_t get_verbosity() const {return verbosity_;}

	/** Returns a string representation of the object used in the log text
	 * Formatted as MessageType:Verbosity
	 */
	string ToString() const;

	/** Returns a comparable value representing how severe the log is.
	 * The high word is the Messsage Type, the low word is the inverse Verbosity
	 */
	constexpr uint16_t GetSeverity() const
	{
		return get_message_type() << 8 | ~get_verbosity();
	}

public:
	/// Implicit conversion to string
	operator string() const {return ToString();}

	/// Implicit conversion to uint16_t
	constexpr operator uint16_t() const {return GetSeverity();}

private:
	Message_t message_type_;   ///< Type of the message
	Verbosity_t verbosity_;    ///< Verbosity of the message
};

/// Formatter for MessageData
std::ostream& operator<< (std::ostream& strm, const MessageData &data);

}  // namespace dman

#endif  // SRC_LOG_MESSAGEDATA_H_
