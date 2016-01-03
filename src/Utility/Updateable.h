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

#ifndef SRC_UTILITY_UPDATEABLE_H_
#define SRC_UTILITY_UPDATEABLE_H_

// STD Includes
#include <chrono>
#include <atomic>
#include <future>

namespace dman
{

/** Abstract interface for a possibly time sensitive Update function
 * A timeout can be defined by derived classes to specify that doUpdate should
 * be run asynchronously on a timeout. In the event of timeout, no more threads
 * will be spawned, and doUpdate won't be called again until the previous
 * call finishes.
 */
class Updateable
{
public:
	using Duration_t = std::chrono::microseconds;
 	using Flag_t = std::atomic<bool>;

public:
	Updateable(): timedout_(false) {}
	Updateable(Updateable&& other):
		timedout_(other.timedout_.load()),
		future_(std::move(other.future_)) {}

	virtual ~Updateable() = default;

public:
	/** Asyncronously calls doUpdate,
	 * so as long the previous doUpdate isn't still running.
	 * @return If true, the update timed out and may still be running.
	 * Otherwise it was successful
	 */
	bool Update();

	/// Returns timeout duration of the update. Returns 0 for no timeout
	virtual Duration_t GetTimeout() const;

	bool HasTimeout() const {return GetTimeout() == Duration_t::zero();}

protected:
	/// Derived functions override this to define an update method
	virtual void doUpdate() = 0;

	/** Returns true if the timeout expired.
	 * Won't return false until the timed out doUpdate finishes
	 */
	bool has_timedout() const {return timedout_.load();}

private:
	Flag_t timedout_;
	std::future<void> future_;

};

}  // namespace dman

#endif  // SRC_UTILITY_UPDATEABLE_H_
