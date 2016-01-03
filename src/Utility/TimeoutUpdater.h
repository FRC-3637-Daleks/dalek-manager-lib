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

#ifndef SRC_UTILITY_TIMEOUTUPDATER_H_
#define SRC_UTILITY_TIMEOUTUPDATER_H_

// Project Includes
#include "Updateable.h"

// STD Includes
#include <chrono>
#include <memory>

namespace dman
{

/** Class which provides interface for setting a timeout stored in the object
 */
class TimeoutUpdater: public Updateable
{
public:
	/// Initializes with timeout
	TimeoutUpdater(const Duration_t& timeout): timeout_(timeout) {}

	TimeoutUpdater(TimeoutUpdater&&) = default;
	virtual ~TimeoutUpdater() = default;

public:
	/// Returns the timeout_ set at construction
	Duration_t GetTimeout() const final {return timeout_;}

private:
	const Duration_t timeout_;
};

/** Maintains another Updateable guaranteeing it's executed on a timeout
 * Requires exclusive ownership of the Updateable. Also requires that the
 * timeout of the managed updater is less than that of the wrapper, or there
 * would be potential for an annoying amount of threads.
 */
class TimeoutWrapper: public TimeoutUpdater
{
public:
	using Stored_t = std::shared_ptr<Updateable>;

public:
	/// Initializes with timeout and other object
	TimeoutWrapper(const Duration_t& timeout, Stored_t&& updater=nullptr):
		TimeoutUpdater(timeout), updater_(std::move(updater)) {}

	TimeoutWrapper(TimeoutWrapper&&) = default;
	virtual ~TimeoutWrapper() = default;

public:
	void set_updater(Stored_t&& updater) {updater_ = std::move(updater);}
	bool has_updater() const {return updater_ != nullptr;}
	Stored_t&& release_updater() {return std::move(updater_);}

	/** Returns true if the wrapper's timeout is greater than that of the
	 * stored updater's. Returns false if the stored updater is null or
	 * has a longer timeout than the wrapper
	 */
	bool ValidUpdater() const;

protected:
	/** Calls doUpdate on the stored updater
	 * @pre \code{.cpp} ValidUpdater() == true \endcode
	 * @exception logic_error Preconditions not met
	 */
	void doUpdate() final;

private:
	Stored_t updater_;
};

}  // namespace dman

#endif  // SRC_UTILITY_TIMEOUTUPDATER_H_
