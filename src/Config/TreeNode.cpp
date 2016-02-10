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
#include "TreeNode.h"

// Boost Includes
#include <boost/range/any_range.hpp>

// STD Includes
#include <stdexcept>

namespace dman
{

bool TreeNode::LoadConfig(const json &config)
{
	bool ret = false;

	auto range = GetRange();
	for(auto i : range)
	{
		if(i.second == nullptr)
			continue;

		auto sub_config = config.find(i.first);
		if(sub_config != config.end())
		{
			// Passes object at key the json value at that key
			ret |= i.second->LoadConfig(*sub_config);
		}
		else
		{
			// Signals missing values
			ret = true;

			// Passes in null json value
			i.second->LoadConfig(nullptr);
		}
	}

	return ret;
}

json TreeNode::GetConfig() const
{
	json ret(json::value_t::object);

	auto range = GetRange();
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

	auto range = GetRange();
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
