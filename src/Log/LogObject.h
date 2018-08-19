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

#ifndef SRC_LOG_LOGOBJECT_H_
#define SRC_LOG_LOGOBJECT_H_

// Project Includes
#include "StreamHandle.h"
#include "MessageData.h"
#include "SystemData.h"

namespace dman
{

/** Holds SystemData passed into Log function
 */
class LogObject
{
public:
	LogObject() = default;
	LogObject(const LogObject&) = default;
	LogObject(LogObject&&) = default;
	LogObject(std::string name): system_name_(std::move(name)) {}

public:
	std::string get_system_name() const {return system_name_;}
	void SetSystemName(std::string name) {system_name_ = std::move(name);}

public:
	StreamHandle Log(const MessageData& message_data = MessageData::INFO,
					 std::string component_name = "",
					std::string component_type = "");

private:
	std::string system_name_;
};


}  // namespace dman

#endif  // SRC_LOG_LOGOBJECT_H_