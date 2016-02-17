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

#ifndef SRC_CONFIG_INVALIDMANIFESTERROR_H_
#define SRC_CONFIG_INVALIDMANIFESTERROR_H_

// STD Includes
#include <stdexcept>
#include <string>

namespace dman
{

class InvalidManifestError: public std::invalid_argument
{
public:
	/// Value in file missing
	InvalidManifestError(
		std::string manifest_file_path,
		std::string missing_value): std::invalid_argument(
		manifest_file_path + " is missing the json value \"" +
		missing_value) {}

	/// File missing
	InvalidManifestError(
		std::string manifest_file_path = "~/dman/dalek/manifest.json"):
		std::invalid_argument(std::string("Could not find manifest file ") +
			manifest_file_path) {}
};

}  // namespace dman

#endif  // SRC_CONFIG_INVALIDMANIFESTERROR_H_
