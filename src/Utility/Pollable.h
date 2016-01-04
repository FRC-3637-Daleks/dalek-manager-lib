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

#ifndef SRC_UTILITY_POLLABLE_H_
#define SRC_UTILITY_POLLABLE_H_

// Project Includes
#include "Valuable.h"
#include "Updateable.h"

namespace dman
{

/** Sets its value via some poll function upon Updating
 */
template<typename T>
class Pollable: public Valuable<T>, public Updateable
{
public:
	virtual ~Pollable() = default;

protected:
	/// Sets the internal value to whatever poll returns
	void doUpdate() final
	{
		this->setValue(std::forward<T>(poll()));
	}

	/// Returns a T
	virtual T&& poll() = 0;
};

}  // namespace dman

#endif  // SRC_UTILITY_POLLABLE_H_
