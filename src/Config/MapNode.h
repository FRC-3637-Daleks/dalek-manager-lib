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
#include <boost/range/adaptor/transformed.hpp>

// STD Includes
#include <map>
#include <memory>
#include <functional>
#include <utility>

namespace dman
{

/** Defines Node which only contains subnodes of type T
 */
template<class T>
class MapNode: public TreeNode
{
public:
	using Node_t = T;
	using Mapped_t = std::shared_ptr<Node_t>;
	using NodeConstructor_t = std::function<Mapped_t()>;
	using Map_t = std::map<Key_t, Mapped_t>;

public:
	/** Constructs a MapNode
	 * @param factory This functor is called whenever a new node is created \\
	 * by default it uses std::make_shared<Node_t> with no arguments.\\
	 * It must return a std::shared_ptr<Node_t> object
	 */
	MapNode(NodeConstructor_t factory = &std::make_shared<Node_t>):
		node_factory_(std::move(factory)) {}

	template<class U> MapNode(const MapNode<U>& other):
		map_(other.map_.begin(), other.map_.end()),
		node_factory_(other.node_factory_) {}

	virtual ~MapNode() = default;

public:
	/** Returns a reference to the Node at key, silently creating it with \\
	 * makeNode if one didn't exist before the call
	 */
	Node_t& operator[](const Key_t& key)
	{
		auto& val = map_[key];
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
	const Node_t& operator[](const Key_t &key) const {return at(key);}

public:
	/** Returns a dereferenced range of the nodes
	 */
	auto GetMapRange()
	{
		 return boost::adaptors::transform(
			 boost::make_iterator_range(map_.begin(), map_.end()),
			 [](const typename Map_t::value_type &pair)
			 {
				 return std::pair<const Key_t&, Node_t&>(
					 pair.first, *pair.second);
			 });
	}
	auto GetMapRange() const
	{
		return boost::adaptors::transform(
			boost::make_iterator_range(map_.cbegin(), map_.cend()),
			[](const typename Map_t::value_type &pair)
			{
				return std::pair<const Key_t&, const Node_t&>(
					pair.first, *pair.second);
			});
	}

	// TODO(EdWard): Create more map_ accessor methods

public:
	/** Returns iterator range to the stored \c map_ as pointers to \c Node s
	 */
	Range_t GetRange() const override
	{
		return boost::adaptors::transform(
			boost::make_iterator_range(map_.begin(), map_.end()),
			[](const typename Map_t::value_type &pair)
			{
				return std::pair<const Key_t&, Node*>(
					pair.first, pair.second.get());
			});
	}

private:
	/** Returns a new object to store in the map when a new node is requested
	 * The default implementation returns a default constructed Mapped_t
	 */
	Mapped_t makeNode() const {return node_factory_();}

private:
	Map_t map_;
	NodeConstructor_t node_factory_;
};

}  // namespace dman

#endif  // SRC_CONFIG_MAPNODE_H_
