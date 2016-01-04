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

#ifndef SRC_UTILITY_VALUABLE_H_
#define SRC_UTILITY_VALUABLE_H_

// Project Includes
#include "Gettable.h"

// STD Includes
#include <atomic>
#include <utility>

namespace dman
{

/** Holds and maintains a threadsafe interface for a value of type T
 */
template<typename T>
class Valuable: public Gettable<T>
{
public:
	/// Default constructor
	Valuable(): value_(T()) {}

	/// Perfect forward constructs
	Valuable(T&& val): value_(std::forward<T>(val)) {}

	/// Copy Constructor
	Valuable(const Valuable<T>& other) = default;

	/// Move Constructor
	Valuable(Valuable<T>&& other) = default;

	/// Destructor
	virtual ~Valuable() = default;

public:
	/// Returns value atomically
	T Get() const final
	{
		return value_;
	}

protected:
	/// Sets the value atomically
	void setValue(T&& val)
	{
		value_ = std::forward<T>(val);
	}

private:
	/// Internal value. I know nothing about atomics. Help me.
	std::atomic<T> value_;
};

}  // namespace dman

#endif  // SRC_UTILITY_VALUABLE_H_
