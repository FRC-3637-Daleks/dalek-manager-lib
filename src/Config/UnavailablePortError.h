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

#ifndef SRC_CONFIG_UNAVAILABLEPORTERROR_H_
#define SRC_CONFIG_UNAVAILABLEPORTERROR_H_

// Project Includes
#include "PortSpace.h"

// STD Includes
#include <stdexcept>
#include <string>

namespace dman
{

/** Exception thrown when an attempt is made to use a non-existent port is made
 */
class UnavailablePortError: public std::out_of_range
{
public:
	/// Returns string representation of the error
	static std::string GetError(const PortSpace::Value_t request,
									   const PortSpace& space);

public:
	UnavailablePortError(const PortSpace::Value_t request,
						 const PortSpace& space):
		std::out_of_range(GetError(request, space)) {}
};

}  // namespace dman

#endif  // SRC_CONFIG_UNAVAILABLEPORTERROR_H_
