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

#ifndef SRC_CONFIG_PORT_H_
#define SRC_CONFIG_PORT_H_

// Project Includes
#include "Node.h"

// STD Includes
#include <memory>

namespace dman
{

class PortSpace;

/** Config Node which defines a Port, which is just an integer essentially.
 * Ports have the option to belong to a Port Space which is set of \\
 * unique ports whose values are between a min value and max value
 */
class Port: public Node
{
public:
	using Value_t = uint16_t;
	using PortSpace_t = std::shared_ptr<PortSpace>;

	/// Equivalent of a NULL port
	static const Value_t empty = 0xffff;

public:
	/** Defines a Port whose value must be unique and within the bounds
	 * defined by \c port_space
	 * Value is initialized to emtpy
	 * The default value will be the first available value within the \\
	 * port space, unless the port space is null in which case the default \\
	 * will be empty as well
	 */
	explicit Port(PortSpace_t port_space);

	/** Defines a Port whose value must be unique and within the bounds
	 * defined by \c port_space. Object also attempts to initialize value with
	 * \c init_value
	 * @pre init_value isn't already being used in the port_space
	 * @exception UnavailablePortError If preconditions aren't met
	 * @post Port's value and default will equal \c init_value if \\
	 * preconditions were met. If an UnavailablePortError was thrown \\
	 * value will be empty and default will be the first available port in \\
	 * the port space
	 */
	Port(PortSpace_t port_space, const Value_t init_value);

	/** Defines a Port whose value can be anything
	 * @param init_value value to initialize the Port
	 */
	explicit Port(const Value_t init_value = empty):
		value_(init_value),
		default_value_(init_value == empty? 0:init_value),
		port_space_(nullptr) {}

	/** It's ambiguous what coyping a Port would naturally be, and the ideal
	 * behavior is too volatile in different scenarios and is too implicit
	 */
	Port(const Port&) = delete;
	Port& operator= (const Port&) = delete;

	Port(Port&&) = default;
	Port& operator= (Port&&) = default;

	/// Releases itself from the port space
	~Port();

public:
	const Value_t get_value() const {return value_;}
	const bool is_empty() const {return value_ == empty;}
	const Value_t get_default() const {return default_value_;}
	const PortSpace_t get_port_space() const {return port_space_;}
	const bool has_port_space() const {return port_space_ != nullptr;}

	/** Returns the current value if it is not empty.
	 * Returns the default value if the value is empty.
	 */
	const Value_t GetValueOrDefault() const;

	/** Sets the default to \c default_value and returns \\
	 * \c GetValueOrDefault()
	 */
	const Value_t GetValueOrDefault(const Value_t default_value);

	/** Attempts the value of the port to \c value
	 * If the port doesn't belong to a port space it's a trivial assignment
	 * If the port belongs to a port space it ensures that \c value is not
	 * already taken within the space. If \c value isn't taken it releases
	 * the port at the previous value and takes the port at \c value
	 * @pre value is not already being used by the port space
	 * @exception UnavailablePortError If preconditions aren't met
	 */
	void SetValue(const Value_t value);

	/** Sets default value to \c value
	 */
	void SetDefault(const Value_t value);

	/** Sets port space to \c port_space
	 * This will first release its port value from the previous port space
	 * It will then attempt to set its value in the new port space. If this
	 * fails it sets its value to empty.
	 */
	void SetPortSpace(PortSpace_t port_space);

public:
	/** Calls set value with number in config.
	 * It will set the port to the default and if the default is \\
	 * unavailable, it will set the port to the first available port if:
	 *  - The config value is a null json object
	 *  - OR 
	 *    -# The Port belongs to a port space
	 *    -# AND The input value is already used by the port space
	 *    -# AND The input value is different from the port's current value
	 * If the current value isn't empty and the above conditions are met then
	 * the port just keeps its current value
	 * @pre config is a json integer, and the stored value isn't already in \\
	 * use, or the config is a json null
	 * @exception std::domain_error If value stored in config is not integer \\
	 * or null
	 * @exception UnavailablePortError If the port isn't available
	 * @see SetValue
	 */
	bool LoadConfig(json config) override;

	/** Returns a json integer containing the current value of the object
	 * If the value is empty it returns a null json value
	 */
	json GetConfig() const override;

	/** Returns a schema for the Port, defining it as a json integer
	 * If it belongs to a port space, the schema will also contain \\
	 * a min and max as defined by the port space
	 */
	json GetSchema() const override;

private:
	Value_t value_;
	Value_t default_value_;
	PortSpace_t port_space_;
};

}  // namespace dman

#endif  // SRC_CONFIG_PORT_H_
