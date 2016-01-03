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
#include "UpdateStore.h"

// STD Includes
#include <future>
#include <chrono>
#include <atomic>
#include <stdexcept>

namespace dman
{

Updateable::Duration_t UpdateStore::GetTimeout() const
{
	if(default_timeout_ == Duration_t::zero() && !zero_updaters_.empty())
	{
		return Duration_t::zero();
	}
	else if (default_timeout_ == Duration_t::zero())
	{
		return TotalElementTimeout();
	}
	else
	{
		return TotalElementTimeout() + TotalDefaultTimeout();
	}
}

Updateable::Duration_t UpdateStore::TotalElementTimeout() const
{
	Duration_t total = Duration_t::zero();
	for(auto updater : timeout_updaters_)
		total += updater->GetTimeout();

	return total;
}

Updateable::Duration_t UpdateStore::TotalDefaultTimeout() const
{
	return get_default() * zero_updaters_.size();
}

void UpdateStore::Add(Stored_t updater)
{
	if(updater == nullptr)
		throw std::invalid_argument("nullptr updater in UpdateStore::Add");

	if(updater->HasTimeout())
		timeout_updaters_.push_back(updater);
	else
		zero_updaters_.push_back(updater);
}

void UpdateStore::doUpdate()
{
	// Cycle through updaters with timeouts
	for(auto updater : timeout_updaters_)
	{
		if(updater->Update())
		{
			// TODO(EdWard): On timeout code
		}
	}

	// Cycle through updaters without timeouts
	for(auto updater = zero_updaters_.begin();
		updater != zero_updaters_.end() && !has_timedout();
		updater++)
	{
		(*updater)->Update();
	}
}

}  // namespace dman
