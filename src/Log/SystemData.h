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

#ifndef SRC_LOG_SYSTEMDATA_H_
#define SRC_LOG_SYSTEMDATA_H_


// STD Includes
#include <string>
#include <iostream>

namespace dman
{

using std::string;

/** Object which contains meta data for a component a log is describing
 * Information contains hierarchy: SystemName "ComponentName":ComponentType
 */
class SystemData
{
public:
	/// Allows implicit conversion from nullptr to the default constructed value
	SystemData(): SystemData("Logging", "", "SystemData") {}
	SystemData(string system,
			   string component_name,
			   string component_type):
						system_(std::move(system)),
						component_name_(std::move(component_name)),
						component_type_(std::move(component_type)) {}
	SystemData(const SystemData& other) = default;
	SystemData(SystemData&& other) = default;

public:
	string get_system() const {return system_;}
	string get_component_name() const {return component_name_;}
	string get_component_type() const {return component_type_;}

	/// Returns a string representation of the object used in the log text
	string ToString() const;

public:
	/// Implicit conversion to string
	operator string() const {return ToString();}

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

/// Formatter for SystemData
std::ostream& operator<< (std::ostream& strm, const SystemData &data);

}  // namespace dman

#endif  // SRC_LOG_SYSTEMDATA_H_
