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

#ifndef SRC_CONFIG_HYBRIDNODE_H_
#define SRC_CONFIG_HYBRIDNODE_H_

// Project Includes
#include "TreeNode.h"
#include "MapNode.h"

// Boost Includes
#include <boost/range/any_range.hpp>
#include <boost/range/join.hpp>

// STD Includes
#include <map>
#include <memory>

namespace dman
{

/** Defines a node which contains either objects or more groups of objects
 * This is implemented as to MapNodes, one which contains object groups,
 * and one which contains leaf objects. The most common use of this is
 * \c BasicHybridNode which passes in itself as the Group type. This allows
 * you to have a tree structure where each node is either a \c Leaf_t or a
 * group of \c Leaf_t s and more groups. Think of the Leaf as a file,
 * and the group as a folder.
 * @see BasicHybridNode
 */
template<class Leaf_t, class Group_t>
class HybridNode: public TreeNode
{
public:
	using MapLeaf_t = MapNode<Leaf_t>;
	using LeafFactory_t = typename MapLeaf_t::NodeConstructor_t;
	using MapGroup_t = MapNode<Group_t>;
	using GroupFactory_t = typename MapGroup_t::NodeConstructor_t;

public:
	HybridNode() = default;

	HybridNode(LeafFactory_t leaf_factory):
		leaves_(std::move(leaf_factory)) {}

	HybridNode(GroupFactory_t group_factory):
		groups_(std::move(group_factory)) {}

	HybridNode(LeafFactory_t leaf_factory,
			   GroupFactory_t group_factory):
			   leaves_(std::move(leaf_factory)),
			   groups_(std::move(group_factory)) {}

	virtual ~HybridNode() = default;

public:
	/// Returns reference to internal leaves MapNode
	MapLeaf_t& Leaves() {return leaves_;}
	const MapLeaf_t& Leaves() const {return leaves_;}

	/// Returns reference to internal groups MapNode
	MapGroup_t& Groups() {return groups_;}
	const MapGroup_t& Groups() const {return groups_;}

	/// Returns reference to leaf at \c key
	Leaf_t& operator() (const Key_t& key) {return leaves_[key];}
	const Leaf_t& operator() (const Key_t& key) const {return leaves_[key];}

	/// Returns reference to group at \c key
	Group_t& operator[] (const Key_t& key) {return groups_[key];}
	const Group_t& operator[] (const Key_t& key) const {return groups_[key];}

public:
	/** Returns joint iterator range to the stored maps
	 */
	Range_t GetRange() const override
	{
		return boost::join(leaves_.GetRange(), groups_.GetRange());
	}

public:
	bool AssembleConfig(const json &config) override
	{
		if(!config.is_object())
			return true;

		bool ret = false;
		for(auto element = config.begin(); element != config.end(); element++)
		{
			if(element.value().is_object())
				ret |= (*this)[element.key()].AssembleConfig(element.value());
			else
				ret |= (*this)(element.key()).AssembleConfig(element.value());
		}

		return ret;
	}

private:
	MapLeaf_t leaves_;
	MapGroup_t groups_;
};

/** Generic HybridNode where each group is itself a BasicHybridNode
 * which uses the default makeLeaf and makeGroup
 */
template<class Leaf_t>
class BasicHybridNode: public HybridNode<Leaf_t, BasicHybridNode<Leaf_t> >
{
public:
	using Self_t = BasicHybridNode<Leaf_t>;
	using Base_t = HybridNode<Leaf_t, Self_t>;
	using MapLeaf_t = typename Base_t::MapLeaf_t;
	using LeafFactory_t = typename Base_t::LeafFactory_t;
	using MapGroup_t = typename Base_t::MapGroup_t;
	using GroupFactory_t = typename Base_t::GroupFactory_t;

public:
	using Base_t::HybridNode;

	/** Each Group created under this object will be passed in leaf_factory \\
	 * and a group factory function which does the same
	 */
	BasicHybridNode(const LeafFactory_t &leaf_factory):
		Base_t(leaf_factory, MakeMakeNode(leaf_factory)) {}
	virtual ~BasicHybridNode() = default;

private:
	static GroupFactory_t MakeMakeNode(
		LeafFactory_t leaf_factory)
	{
		return [leaf_factory]() {return MakeNode(leaf_factory);};
	}

	static std::shared_ptr<Base_t> MakeNode(
		const LeafFactory_t &leaf_factory)
	{
		return std::make_shared<Base_t>(leaf_factory,
										MakeMakeNode(leaf_factory));
	}
};

}  // namespace dman

#endif  // SRC_CONFIG_HYBRIDNODE_H_
