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

#ifndef SRC_UTILITY_UPDATETHREAD_H_
#define SRC_UTILITY_UPDATETHREAD_H_

// Project Includes
#include "Updateable.h"

// STD Includes
#include <thread>
#include <atomic>
#include <chrono>
#include <memory>

namespace dman
{

/** Manages an Updateable on which it calls Update at a set period in a thread
 * The thread sleeps for as little as \c GetRestPeriod() every loop, however, it
 * will sleep longer if the Updateable returns sooner than its timeout. This
 * guarantees the Update will be called in even intervals. If no period is set,
 * the thread will only yield every loop, but otherwise, won't rest.
 */
class UpdateThread
{
public:
	using Stored_t = std::shared_ptr<Updateable>;
	using Duration_t = Updateable::Duration_t;

public:
	/// Initializes with no period
	UpdateThread(): UpdateThread(Duration_t::zero()) {}

	UpdateThread(const Duration_t period, Stored_t updater = nullptr):
		period_(period), updater_(std::move(updater)), running_(false) {}

	UpdateThread(const UpdateThread&) = delete;
	UpdateThread(UpdateThread&&) = default;

	/** Will call \c Stop() if the thread is running
	 * @see UpdateThread::Stop
	 */
	~UpdateThread();

public:
	/** Sets the updater.
	 * @pre \code{.cpp} is_running() == false \endcode
	 * @exception logic_error Thread was running
	 */
	void SetUpdater(Stored_t updater);

	/** Sets the period
	 * @pre \code{.cpp} is_running() == false \endcode
	 * @exception logic_error Thread was running
	 */
	void SetPeriod(const Duration_t period);

	bool has_updater() const {return updater_ != nullptr;}
	Duration_t get_period() const {return period_;}

	/// Returns the period of the updater. Returns zero if the updater is null.
	Duration_t GetUpdaterPeriod() const;

	/// Returns the minimum period the thread will rest between updates
	Duration_t GetRestPeriod() const;

public:
	/** Starts the thread which calls the updater's \c Update()
	 * @pre \code{.cpp} has_updater() == true \endcode
	 * @pre \code{.cpp} is_running() == false \endcode
	 * @post \code{.cpp} is_running() == true \endcode
	 * @exception logic_error Preconditions aren't met.
	 */
	void Start();

	/// Returns true if the thread is running
	bool is_running() const {return running_.load();}

	/** Stops the loop and joins the thread.
	 * This action can take up to \c get_period() amount of time
	 * @pre \code{.cpp} is_running() == true \endcode
	 * @post \code{.cpp} is_running() == false \endcode
	 * @exception logic_error Preconditions aren't met.
	 */
	void Stop();

private:
	void threadFunc();

private:
	Stored_t updater_;
	Duration_t period_;
	std::thread thread_;
	std::atomic<bool> running_;
};

}  // namespace dman

#endif  // SRC_UTILITY_UPDATETHREAD_H_
