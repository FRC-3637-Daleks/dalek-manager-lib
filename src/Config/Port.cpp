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
#include "Port.h"
#include "PortSpace.h"
#include "UnavailablePortError.h"

namespace dman
{

Port::Port(PortSpace_t port_space):
	value_(empty),
	default_value_(empty),
	port_space_(port_space)
{
	if(has_port_space())
		default_value_ = get_port_space()->GetNextDefault();
}

Port::Port(PortSpace_t port_space, const Value_t init_value):
	port_space_(port_space)
{
	try
	{
		if(has_port_space())
		{
			port_space_->Use(init_value);
		}
		value_ = init_value;
		default_value_ = init_value;
	}
	catch(const UnavailablePortError& upe)
	{
		value_ = empty;
		default_value_ = get_port_space()->GetNextDefault();
		throw UnavailablePortError(upe);
	}
}

Port::~Port()
{
	if(has_port_space())
		port_space_->Release(value_);
}

const Port::Value_t Port::GetValueOrDefault() const
{
	if(get_value() == empty)
		return get_default();
	else
		return get_value();
}

const Port::Value_t Port::GetValueOrDefault(const Value_t default_value)
{
	SetDefault(default_value);
	return GetValueOrDefault();
}

void Port::SetValue(const Value_t value)
{
	if(value == value_)
		return;

	if(has_port_space())
	{
		port_space_->Use(value);
		port_space_->Release(value_);
	}

	value_ = value;
}

void Port::SetDefault(const Value_t value)
{
	default_value_ = value;
}

void Port::SetPortSpace(PortSpace_t port_space)
{
	if(has_port_space())
		port_space_->Release(value_);

	port_space_ = std::move(port_space);

	if(has_port_space())
	{
		try
		{
			port_space_->Use(value_);
		}
		catch(const UnavailablePortError& e)
		{
			value_ = empty;
		}
	}
}

bool Port::LoadConfig(const json &config)
{
	auto val = config.get<const json::number_integer_t*>();
	if(val == nullptr || (
		has_port_space() &&  // it has a port space
		get_port_space()->IsUsed(*val) &&  // that value in the space is used
		get_value() != *val)  // that value isn't its own value
	)
	{
		if(get_value() == empty)  // the value is empty so we set it to defaults
		{
			try
			{
				SetValue(get_default());
			}
			catch(const UnavailablePortError& upe)
			{
				SetValue(get_port_space()->GetAvailable());
			}
		}
		else
		{
			// otherwise keep the current value
		}
		return true;
	}
	else
	{
		SetValue(*val);
	}

	return false;
}

json Port::GetConfig() const
{
	if(get_value() == empty)
		return json(nullptr);
	else
		return json(get_value());
}

json Port::GetSchema() const
{
	json ret(json::value_t::object);

	ret["type"] = "integer";
	ret["default"] = default_value_;
	if(has_port_space())
	{
		ret["minimum"] = port_space_->get_min();
		ret["maximum"] = port_space_->get_max();
	}

	return ret;
}

}  // namespace dman
