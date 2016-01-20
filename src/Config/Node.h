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

#ifndef SRC_CONFIG_NODE_H_
#define SRC_CONFIG_NODE_H_

// Lib Includes
#include "json.hpp"

namespace dman
{

using json = nlohmann::json;

/** Abstract base class to represent a configuration node loaded from json
 * Derived classes intended to be strung together in a tree structure
 */
class Node
{
public:
	virtual ~Node() = default;

public:
	/** Loads data from json value
	 * @param config json value this node represents
	 * @return true if the data in \c config was sufficient to load all data
	 */
	virtual bool LoadConfig(json config) = 0;

	/** Returns json representation of data represented by this node
	 * @return json object representing the configuration of this node
	 */
	virtual json GetConfig() const = 0;

	/** Returns json schema detailing valid json this node can load
	 * @return json object representing schema
	 */
	virtual json GetSchema() const = 0;
};

}  // namespace dman

#endif  // SRC_CONFIG_NODE_H_
