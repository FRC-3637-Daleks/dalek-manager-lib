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

#ifndef SRC_CONFIG_SETTING_H_
#define SRC_CONFIG_SETTING_H_

// Project Includes
#include "Node.h"

namespace dman
{

/** Defines a general setting stored as a json value with static type
 * The type used can only be set once and it is set in stone after
 */
class Setting: public Node
{
public:
    static const char * const JSONTypeToString(json::value_t type);
	static bool CanConvert(json::value_t from, json::value_t to);

public:
    /// Type must remain constant if non-null.
    explicit Setting(json::value_t type = json::value_t::null);
    virtual ~Setting() = default;

    /// Moved from object holds null json value after call
    Setting(Setting&&) = default;

    /// Only way to change the internal type
    Setting& operator=(Setting&&) = default;
    Setting& operator=(const Setting& other) = default;

public:
    /// Returns if there is no value set
    bool is_empty() const {return value_.is_null();}

    /// Returns if no type hsa been set
    bool has_no_type() const {return type_ == json::value_t::null;}

    /// Returns the data type of this Setting
    json::value_t get_type() const {return type_;}

    /// Returns the current internal value
    json get_value() {return value_;}
    const json& get_value() const {return value_;}
    template<typename T>
    T get_value() const
    {
		return get_value().get<T>();
	}

    /// Returns the current default value
    json get_default() {return default_value_;}
    const json& get_default() const {return default_value_;}
    template<typename T>
    T get_default() const {return get_default().get<T>();}

    /// Returns the current value, if this is null, returns the default value
    json GetValueOrDefault();
    const json& GetValueOrDefault() const;
    template<typename T> T GetValueOrDefault() const
    {
        if (is_empty())
            return get_default<T>();
        else
            return get_value<T>();
    }

    /** Attempts to set the value of the setting to \c value
     * @pre \code{.cpp} value.type() == get_type() \endcode
     * @exception SettingMismatchTypeError
     */
    void SetValue(json value);

    /** Attempts to set the value of the default setting to \c default_value
     * @pre \code{.cpp} defualt_value.type() == get_type() \endcode
     * @exception SettingMistmatchTypeError
     */
    void SetDefault(json default_value);

public:
    /** Sets values to config. Sets value to default if type mismatch
     * @return true if config did not match internal type
     */
    bool LoadConfig(const json &config) override;

    /** Returns current json value
     */
    json GetConfig() const override;

    /** Returns a schema for the Setting
     * Specifies it as a json value of the same type \\
     * with get_default() as the default value
     */
    json GetSchema() const override;

private:
    json value_;
    json default_value_;
    json::value_t type_;
};


}  // namespace dman


#endif  // SRC_CONFIG_SETTING_H_
