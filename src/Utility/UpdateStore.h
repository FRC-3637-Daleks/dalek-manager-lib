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

#ifndef SRC_UTILITY_UPDATESTORE_H_
#define SRC_UTILITY_UPDATESTORE_H_

// Project Includes
#include "Updateable.h"

// STD Includes
#include <vector>
#include <chrono>

namespace dman
{

/** Holds a vector of pointers to Updateable's and Updates them together
 * Is itself an Updateable, so Updateable's can be nested.
 * Also allows user to define what happens on blocks and timeouts
 */
class UpdateStore: public Updateable
{
public:
	using Stored_t = Updateable *;
	using Container_t = std::vector<Stored_t>;

public:
	/// Initializes default timeout to zero
	UpdateStore(): default_timeout_(Duration_t::zero()) {}

	UpdateStore(const Duration_t &default_timeout):
		default_timeout_(default_timeout) {}

	UpdateStore(const UpdateStore&) = delete;
	UpdateStore(UpdateStore&&) = default;
	virtual ~UpdateStore() = default;

public:
	/** Returns timeout based on the default timeout and element timeouts
	 * If \c default_timeout_ is zero and \c zero_updaters_ has elements, this
	 * will return zero. If \c default_timeout_ is zero and \c zero_updaters_
	 * doesn't have elements, GetTimeout will return the sum of 
	 * \c timeout_updaters_ ' elements' timeouts. If \c default_timeout_ is
	 * nonzero, than this will return the sum of \c timeout_updaters_ '
	 * elements' timeouts and the number of elements in \c zero_updaters_ times
	 * the \c default_timeout_ .
	 */
	Duration_t GetTimeout() const override;

	/// Returns total timeout for elements that have a timeout
	Duration_t TotalElementTimeout() const;

	/// Returns total timeout due to default_timeout_
	Duration_t TotalDefaultTimeout() const;

	void set_default(Duration_t dur) {default_timeout_ = std::move(dur);}
	Duration_t get_default() const {return default_timeout_;}

public:
	/** Adds \c updater to the list of updaters called by the store's \c Update
	 * If the updater has a nonzero timeout it will be added to a list of
	 * updaters whose \c Update s are called first and checked for timeouts.
	 * If the updater has a zero-timeout it will be added to a list of
	 * updaters whose \c Update s are called last and unchecked
	 */
	void Add(Stored_t updater);

protected:
	/// Updates everything in the list
	void doUpdate() final;

private:
	/// Updateables with no timeout
	Container_t zero_updaters_;

	/// Updateables with a timeout
	Container_t timeout_updaters_;

	/// Default timeout Updateables with no timeout of their own represent
	Duration_t default_timeout_;
};

}  // namespace dman

#endif  // SRC_UTILITY_UPDATESTORE_H_
