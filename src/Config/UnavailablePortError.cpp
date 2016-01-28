/*
    Copyright (C) 2016, EdWard <ezeward4@gmail.com>
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
#include "UnavailablePortError.h"

// STD Includes
#include <string>
#include <sstream>

namespace dman
{

std::string UnavailablePortError::GetError(const PortSpace::Value_t request,
										   const PortSpace& space)
{
	std::stringstream ret;

	if(request < space.get_min())
	{
		ret << "Port request (" << request <<
			") was less than the port space's lower bound (" <<
			space.get_min() << ")";
	}
	else if (request > space.get_max())
	{
		ret << "Port request (" << request <<
			") was greater than the port space's upper bound (" <<
			space.get_max() << ")";
	}
	else if (space.IsUsed(request))
	{
		ret << "Port Request (" << request <<
			") is already held by another Port";
	}
	else if (space.GetAvailable() == PortSpace::empty)
	{
		ret << "Port Space is already full";
	}
	else
	{
		return "Unknown port space error";
	}

	return ret.str();
}

}  // namespace dman
