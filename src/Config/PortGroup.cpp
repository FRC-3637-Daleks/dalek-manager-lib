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
#include "PortGroup.h"

// STD Includes
#include <memory>

namespace dman
{

PortGroup::PortGroup(PortSpace_t port_space):
			// leaf factory function
	Base_t([this]() {return std::make_shared<Port>(port_space_);},
		   // group factory function
		   [this]() {return std::make_shared<PortGroup>(port_space_);}
	),
	port_space_(std::move(port_space)) {}

void PortGroup::SetPortSpace(PortSpace_t port_space)
{
	port_space_ = std::move(port_space);
}

void PortGroup::PropogatePortSpace(PortSpace_t port_space)
{
	SetPortSpace(std::move(port_space));
	PropogatePortSpace();
}

void PortGroup::PropogatePortSpace()
{
	{
		auto leaves = Leaves().GetMapRange();
		for(auto leaf : leaves)
			leaf.second.SetPortSpace(port_space_);
	}

	{
		auto groups = Groups().GetMapRange();
		for(auto group : groups)
			group.second.PropogatePortSpace(port_space_);
	}
}



}  // namespace dman
