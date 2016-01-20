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

#ifndef SRC_CONFIG_MAPNODE_H_
#define SRC_CONFIG_MAPNODE_H_

// Project Includes
#include "TreeNode.h"

// Boost Includes
#include <boost/range/any_range.hpp>

// STD Includes
#include <map>
#include <memory>

namespace dman
{

template<class T>
class MapNode: public TreeNode
{
public:
	using Node_t = T;

public:
	template<class U> MapNode(const MapNode<U> other):
		map_(other.map_.begin(), other.map_.end()) {}
	virtual ~TreeNode() = default;

public:
	/** Returns a reference to the Node at key, silently creating it with \\
	 * makeNode if one didn't exist before the call
	 */
	Node_t& operator[](const Key_t key)
	{
		auto val = map_[key];
		if(val == nullptr)
			val = makeNode();
		return *val;
	}

	/** Returns a reference to the Node at key, complaining via exceptions \\
	 * if one didn't exist before the call
	 * @pre There is already a value at \c key
	 * @exception out_of_range Thrown if precondition not met
	 */
	Node_t& at(const Key_t& key) {return *map_.at(key);}
	const Node_t& at(const Key_t& key) const
	{
		return const_cast<MapNode<T>*>(this)->at(key);
	}
	const Node_t& operator[](const Key_t key) const {return at(key);}

	//TODO(EdWard): Create more map_ accessor methods

protected:
	using Mapped_t = std::unique_ptr<Node_t>;
	using Map_t = std::map<Key_t, Mapped_t>;

private:
	/** Returns iterator range to the stored \c map_
	 */
	Range_t getRange() const override
	{
		return Range_t(map_.begin(), map_.end());
	}

	/** Returns a new object to store in the map when a new node is requested
	 * The default implementation returns a default constructed Mapped_t
	 */
	virtual Mapped_t makeNode() const {return std::make_unique<Node_t>();}

private:
	Map_t map_;
};

}  // namespace dman

#endif  // SRC_CONFIG_MAPNODE_H_