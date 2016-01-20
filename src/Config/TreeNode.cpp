/*
    Copyright (C) 2016, EdWard <dman>
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
#include "TreeNode.h"

// Boost Includes
#include <boost/range/any_range.hpp>

// STD Includes
#include <stdexcept>

namespace dman
{

bool TreeNode::LoadConfig(json config)
{
	bool ret = false;

	auto range = getRange();
	for(auto i : range)
	{
		if(i.second == nullptr)
			continue;

		try
		{
			// Attempts to pass to object the elment at the same key
			ret |= i.second->LoadConfig(config.at(i.first));
		}
		catch (std::out_of_range& e)
		{
			// Signals failure
			ret = true;
		}
	}

	return ret;
}

json TreeNode::GetConfig() const
{
	json ret(json::value_t::object);

	auto range = getRange();
	for(auto i : range)
	{
		if(i.second == nullptr)
			continue;
		ret[i.first] = i.second->GetConfig();
	}

	return ret;
}

json TreeNode::GetSchema() const
{
	json ret(json::value_t::object);

	ret["type"] = "object";
	ret["additionalProperties"] = false;

	auto& required = (ret["required"] = json::array());
	auto& properties = (ret["properties"] = json::object());

	auto range = getRange();
	for(auto i : range)
	{
		if(i.second == nullptr)
			continue;
		required.push_back(i.first);
		properties[i.first] = i.second->GetSchema();
	}

	return ret;
}

}  // namespace dman