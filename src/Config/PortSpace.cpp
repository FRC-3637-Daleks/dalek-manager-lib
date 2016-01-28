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

#include "PortSpace.h"
#include "UnavailablePortError.h"

namespace dman
{

const bool PortSpace::IsUsed(const Value_t port) const
{
	if(port < min_ || port > max_)
		return true;

	return getPort(port);
}

void PortSpace::Use(const Value_t port)
{
	if(IsUsed(port))
		throw UnavailablePortError(port, *this);

	setPort(port, true);
}

void PortSpace::Release(const Value_t port)
{
	if(port < min_ || port > max_ || IsUsed(port) == false)
		return;

	setPort(port, false);
}

const PortSpace::Value_t PortSpace::GetAvailable() const
{
	for(Value_t port = min_; port <= max_; port++)
	{
		if(!IsUsed(port))
			return port;
	}

	return empty;
}

const bool PortSpace::getPort(const Value_t port) const
{
	if(port < min_ || port > max_)
		return true;

	return space_[port - min_];
}

void PortSpace::setPort(const Value_t port, const bool state)
{
	if(port < min_ || port > max_)
		return;

	space_[port - min_] = state;
}

}  // namespace dman
