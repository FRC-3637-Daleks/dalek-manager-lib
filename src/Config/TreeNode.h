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

#ifndef SRC_CONFIG_TREENODE_H_
#define SRC_CONFIG_TREENODE_H_

// Project Includes
#include "Node.h"

// Boost Includes
#include <boost/range/any_range.hpp>

// STD Includes
#include <string>
#include <utility>

namespace dman
{

/** Configuration node which is represented by a json object
 * or a series of subnodes
 */
class TreeNode: public Node
{
public:
	using Key_t = std::string;
	using Mapped_base_t = Node *;
	using MapElement_t = std::pair<const Key_t, Mapped_base_t>;
	using MapElementReference_t = std::pair<const Key_t&, Mapped_base_t>;
	using Range_t = boost::any_range<MapElementReference_t,
									boost::single_pass_traversal_tag,
									MapElementReference_t,
									std::ptrdiff_t>;

public:
	virtual ~TreeNode() = default;

public:
	/** Looks up json object at the key for each item in \c GetRange() \\
	 * and passes into that item's LoadConfig. If the object is lacking a \\
	 * key it will add it to a separate list which will be passed null json \\
	 * values.
	 * @pre \code{.cpp} config.is_object() == true \endcode
	 * @return Returns true if any of the sub objects returned true, \\
	 * or there wasn't a member in config at a required key
	 */
	bool LoadConfig(const json &config) override;

	/** Returns json object formed with the json value returned by each item \\
	 * in \c GetRange()
	 * @return Returns a json object
	 */
	json GetConfig() const override;

	/** Returns json object representing a schema with a required property \\
	 * for each item in \c GetRange() formed from the schema that item returns
	 * @return Returns a json object representing the schema
	 */
	json GetSchema() const override;

public:
	/** Returns range of any forward iterator implementation.
	 */
	virtual Range_t GetRange() const = 0;
};

}  // namespace dman

#endif  // SRC_CONFIG_TREENODE_H_
