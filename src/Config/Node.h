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

// STD Includes
#include <string>

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
	enum SchemaFormat_t {
		color = 0,
		date,
		datetime,
		datetime_local,
		email,
		month,
		number,
		range,
		tel,
		text,
		textarea,
		time,
		url,
		week,
		checkbox,
		table,
		tabs,
		select,
		grid,
		n_formats
	};

	static const char * const schema_formats[n_formats];
	static const char * const get_format_string(SchemaFormat_t format);

public:
	Node();
	virtual ~Node() = default;

public:
	/** Loads data from json value
	 * @param config json value this node represents
	 * @return true if the data in \c config wasn't sufficient to load all data
	 */
	virtual bool LoadConfig(const json &config) = 0;

	/** Derived classes should override this to generate nodes for every \\
	 * element in \c config
	 * By default it just passes the config to \c Node::LoadConfig
	 * @return Result of \c Node::LoadConfig(std::move(config))
	 */
	virtual bool AssembleConfig(const json &config)
	{
		return LoadConfig(config);
	}

	/** Returns json representation of data represented by this node
	 * @return json object representing the configuration of this node
	 */
	virtual json GetConfig() const = 0;

	/** Returns json schema detailing valid json this node can load
	 * The default implementation of this function returns the base_schema_
	 * @return json object representing schema
	 */
	virtual json GetSchema() const {return base_schema_;}

public:
	/// The description shows up below the title/key in the editor
	std::string GetDescription() const;
	void SetDescription(std::string description);

	/// The format defines how the input will look on the screen
	std::string GetFormat() const;
	void SetFormat(std::string format);
	void SetFormat(SchemaFormat_t format);
	bool IsFormat(SchemaFormat_t format) const;

public:
	/// Base Schema returned by the default form of GetSchema
	json base_schema_;
};

}  // namespace dman

#endif  // SRC_CONFIG_NODE_H_
