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
#include "TimeoutUpdater.h"

// STD Includes
#include <stdexcept>
#include <memory>

namespace dman
{

bool TimeoutWrapper::ValidUpdater() const
{
	if(!has_updater())
		return false;

	return GetTimeout() >= updater_->GetTimeout();
}

void TimeoutWrapper::doUpdate()
{
	if(!has_updater())
		throw std::logic_error(
			"TimeoutWrapper::doUpdate() - Wrapper does not have updater");
	else if (!ValidUpdater())
		throw std::logic_error(
			"TimeoutWrapper::doUpdate() - Wrapper's timeout shorter than"
			" stored updater");

	// We don't care about internal timeouts since it has more time anyway
	updater_->Update();
}

}  // namespace dman
