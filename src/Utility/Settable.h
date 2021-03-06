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

#ifndef SRC_UTILITY_SETTABLE_H_
#define SRC_UTILITY_SETTABLE_H_

namespace dman
{

/** Abstract interface for an object which exposes an underlying T value through Set(T)
 * Defines a polymorphic interface for Set(), allowing this to be stored in a list of
 * objects for which the Set function is supported
 */
template<typename T>
class Settable
{
public:
	virtual ~Settable() = default;

public:
	/// Assigns underlying value.
	virtual void Set(T&& val) = 0;
};

}  // namespace dman

#endif  // SRC_UTILITY_SETTABLE_H_
