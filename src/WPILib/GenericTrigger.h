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

#ifndef SRC_WPILIB_GENERICTRIGGER_H_
#define SRC_WPILIB_GENERICTRIGGER_H_

// STD Includes
#include <functional>

// WPI Includes
#include "WPILib.h"

// Project Includes
#include "Utility/ValueStore.h"

namespace dman
{

/** Trigger which wraps around a functor returning a bool.
 */
class GenericTrigger: public Trigger
{
public:
	using TriggerFn_t = std::function<bool()>;

public:
	GenericTrigger(TriggerFn_t fn): func_(std::move(fn)) {}
	GenericTrigger(ValueStore::Value<bool> value):
		func_([value]() {return value.GetValueOr(false);}) {}

public:
	TriggerFn_t get_trigger() const {return func_;}

public:
	bool Get() override;

private:
	TriggerFn_t func_;
};

}  // namespace dman

#endif  // SRC_WPILIB_GENERICTRIGGER_H_
