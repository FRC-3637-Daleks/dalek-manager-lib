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

#ifndef SRC_UTILITY_SETVALUE_H_
#define SRC_UTILITY_SETVALUE_H_

// Project Includes
#include "Valuable.h"
#include "Settable.h"

namespace dman
{

/** Makes Valuable's set function public, letting user explicitly define what values go in
 */
template<typename T>
class SetValue: public Valuable<T>, public Settable<T>
{
public:
	/// Use Valuable's constructorss
	using Valuable<T>::Valuable;

	virtual ~SetValue() = default;

public:
	void Set(T&& val) final
	{
		this->setValue(std::forward<T>(val));
	}
};

}  // namespace dman

#endif  // SRC_UTILITY_SETVALUE_H_
