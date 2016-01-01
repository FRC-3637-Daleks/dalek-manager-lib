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

#ifndef SRC_UTILITY_FUNKYPOLL_H_
#define SRC_UTILITY_FUNKYPOLL_H_

// Project Includes
#include "Pollable.h"

// STD Includes
#include <functional>

namespace dman
{

/** Pollable which polls via a function supplied at construction time
 * The function is guaranteed to not be called until Update is called
 */
template<typename T>
class FunkyPoll: public Pollable<T>
{
public:
	using Func_t = std::function<T()>;

public:
	/// Constructs with a user defined function
	FunkyPoll(Func_t fn): fn_(std::move(fn)) {}

	/// Copy Constructor
	FunkyPoll(const FunkyPoll&) = default;

	/// Move Constructor
	FunkyPoll(FunkyPoll&&) = default;

	virtual ~FunkyPoll() = default;

protected:
	/// Overrides Poll to return the result of a call to the function
	T&& poll() final
	{
		return std::move(fn_());
	}

private:
	Func_t fn_;
};

}  // namespace dman

#endif  // SRC_UTILITY_FUNKYPOLL_H_
