/*
    Copyright (C) 2016 EdWard <ezeeward4@gmail.com>
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

#ifndef SRC_CONFIG_PORTGROUP_H_
#define SRC_CONFIG_PORTGROUP_H_

// Project Includes
#include "Port.h"
#include "HybridNode.h"

// STD Includes
#include <memory>

namespace dman
{

/** Class representing a port group. Contains either Ports or more PortGroups
 * When a subnode of a PortGroup is created it will propogate its PortSpace
 * into the subnode, which will in turn propogate that PortSpace into its
 * subnodes.
 */
class PortGroup: public HybridNode<Port, PortGroup>
{
public:
	using PortSpace_t = std::shared_ptr<PortSpace>;
	using PortSpace_const_t = std::shared_ptr<const PortSpace>;
	using Base_t = HybridNode<Port, PortGroup>;

public:
	/** Constructs PortGroup which is a member of the Port Space \c port_space
	 */
	explicit PortGroup(PortSpace_t port_space = nullptr);
	virtual ~PortGroup() = default;

public:
	/// Returns the port space to which this group is a member
	PortSpace_t get_port_space() {return port_space_;}
	PortSpace_const_t get_port_space() const {return port_space_;}
	bool has_port_space() const {return port_space_ != nullptr;}

	/** Sets the Port Space to which this group belongs
	 * @warning This will not propogate this space to subnodes.
	 * @see PortGroup::PropogatePortSpace
	 */
	void SetPortSpace(PortSpace_t port_space);

	/** Sets the Port Space of the group and all subnodes to match
	 * @param port_space New subnode
	 * @post The PortSpace to which this group and all subnodes belong is \\
	 * \c port_space
	 */
	void PropogatePortSpace(PortSpace_t port_space);

	/** Sets the Port Space of all subnodes to match this group's current space
	 * @post The PortSpace to which all subnodes of this group belong is \\
	 * \c get_port_space()
	 */
	void PropogatePortSpace();

private:
	PortSpace_t port_space_;
};



}  // namespace dman

#endif  // SRC_CONFIG_PORTGROUP_H_
