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

#ifndef SRC_UTILITY_FUNKYGET_H_
#define SRC_UTILITY_FUNKYGET_H_

// Project Includes
#include "Gettable.h"

// STD Includes
#include <functional>

namespace dman
{

/** Gettable which calls the internal function supplied at construction time
 * The function is guaranteed to not be called until Get is called
 */
template<typename T>
class FunkyGet: public Gettable<T>
{
public:
	using Func_t = std::function<T()>;

public:
	/// Constructs with a user defined function
	FunkyGet(Func_t fn): fn_(std::move(fn)) {}

	/// Move Constructor
	FunkyGet(FunkyGet&&) = default;

	virtual ~FunkyGet() = default;

public:
	T Get() final
	{
		return fn_();
	}

private:
	Func_t fn_;
};



}  // namespace dman

#endif  // SRC_UTILITY_FUNKYGET_H_