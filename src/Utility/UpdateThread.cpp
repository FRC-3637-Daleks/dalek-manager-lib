/*
    Copyright (C) 2016, EdWard <ezeward4@gmail.com>
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

// Project Includes
#include "UpdateThread.h"
#include "TimeoutUpdater.h"

// STD Includes
#include <memory>
#include <thread>
#include <atomic>
#include <stdexcept>

namespace dman
{

UpdateThread::~UpdateThread()
{
	if(is_running())
		Stop();
}

void UpdateThread::SetUpdater(Stored_t&& updater)
{
	if(is_running())
		throw std::logic_error(
			"UpdateThread::SetUpdater - Thread was already running");

	updater_ = std::move(updater);
}

void UpdateThread::SetPeriod(const Duration_t period)
{
	if(is_running())
		throw std::logic_error(
			"UpdateThread::SetPeriod - Thread was already running");

	period_ = period;
}

UpdateThread::Duration_t UpdateThread::GetUpdaterPeriod() const
{
	if(!has_updater())
		return Duration_t::zero();

	return updater_->GetTimeout();
}

UpdateThread::Duration_t UpdateThread::GetRestPeriod() const
{
	return get_period() - GetUpdaterPeriod();
}

void UpdateThread::Start()
{
	if(is_running())
		throw std::logic_error(
			"UpdateThread::Start(): Thread was already running");

	if(!has_updater())
		throw std::logic_error(
			"Attempt to start UpdateThread without updater");

	if(!updater_->HasTimeout())
	{
		// Move old updater into wrapper
		auto wrapper_updater =
			std::make_unique< TimeoutWrapper >(period_, std::move(updater_));

		updater_ = std::move(wrapper_updater);
	}

	running_.store(true);
	thread_ = std::thread(&UpdateThread::threadFunc, this);
}

void UpdateThread::Stop()
{
	if(!is_running())
		throw std::logic_error(
			"UpdateThread::Stop(): No thread was running");

	running_.store(false);
	thread_.join();
}

void UpdateThread::threadFunc()
{
	using clock = std::chrono::steady_clock;

	clock::time_point next = clock::now();

	while(running_.load())
	{
		next += period_;
		if(updater_->Update())
		{}  // Timeout occurred

		if(next > clock::now())
			std::this_thread::sleep_until(next);
		else
			std::this_thread::yield();
	}
}



}  // namespace dman
