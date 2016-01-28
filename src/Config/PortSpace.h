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

#ifndef SRC_CONFIG_PORTSPACE_H_
#define SRC_CONFIG_PORTSPACE_H_

// Project Includes
#include "Port.h"

// STD Includes
#include <vector>

namespace dman
{

/** Struct which maintains available ports in a port space
 * Ports make requests to take and release ports in the space
 */
class PortSpace
{
public:
	using Value_t = Port::Value_t;
	using Space_t = std::vector<bool>;

	/// @see Port::empty
	static const Value_t empty = Port::empty;

public:
	PortSpace(const Value_t min, const Value_t max):
		min_(min), max_(max), space_(max-min+1, false) {}

public:
	const Value_t get_min() const {return min_;}
	const Value_t get_max() const {return max_;}

	/** Returns true if the port at \c port is in use by another Port
	 * Will also return true if the port is outside the port space's range
	 */
	const bool IsUsed(const Value_t port) const;

	/** Sets the port at \c port in the space to true signifiying it's in use
	 * @pre \code{.cpp} IsUsed(port) == false && min < port < max \endcode
	 * @post \code{.cpp} IsUsed(port) == true \endcode
	 * @exception UnavailablePortError If preconditions aren't met
	 */
	void Use(const Value_t port);

	/** Ensures the port at \c port is available
	 * @post \code{.cpp} IsUsed(port) == false \endcode
	 */
	void Release(const Value_t port);

	/** Returns the first port which is not currently in use
	 * Returns Port::empty if no ports in the space are available
	 */
	const Value_t GetAvailable() const;

private:
	const bool getPort(const Value_t port) const;
	void setPort(const Value_t port, const bool state);

private:
	Value_t min_;
	Value_t max_;
	Space_t space_;
};

}  // namespace dman

#endif  // SRC_CONFIG_PORTSPACE_H_
