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
#include "Updateable.h"

// STD Includes
#include <future>
#include <chrono>
#include <atomic>

namespace dman
{

bool Updateable::Update()
{
	if(timedout_.load())  //< Checks if timed out last call
	{
		// Sees if the value is ready since the last call.
		if(future_.wait_for(Duration_t::zero()) == std::future_status::timeout)
		{
			return true;
		}
		else
		{
			timedout_.store(false);
		}
	}

	auto timeout = GetTimeout();
	if(timeout == std::chrono::microseconds::zero())
	{
		doUpdate();
		return false;
	}

	future_ = std::async(std::launch::async, &Updateable::doUpdate, this);
	auto ret = future_.wait_for(timeout) == std::future_status::timeout;
	if(!ret)
		future_.get();  //< generates exception if doUpdate threw an exception
	else
	{
		//  Attempts to stop the task ASAP if its checking
		timedout_.store(true);
	}

	return ret;
}

std::chrono::microseconds Updateable::GetTimeout() const
{
	return std::chrono::microseconds::zero();
}

}  // namespace dman
