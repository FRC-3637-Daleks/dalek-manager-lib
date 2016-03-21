/*
    Copyright (C) 2016 EdWard <>
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

#ifndef SRC_CONFIG_SETTINGWRAPPER_H_
#define SRC_CONFIG_SETTINGWRAPPER_H_

// Project Includes
#include "Setting.h"

// STD Includes
#include <memory>
#include <stdexcept>

namespace dman
{

/** Returns the default schema format for the type T
 * generic: number
 * string/c-string: text
 * bool: checkbox
 */
template<typename T>
constexpr Node::SchemaFormat_t GetDefaultSchemaFormat();

using NullSettingError = std::logic_error;

/** Class which maintains a reference to a setting
 * Good to use in Systems which simply set member variables to a setting \\
 * at configure time. Also allows for extensions upon the core setting schema
 * @tparam T Consistent Type which this setting will act as
 */
template<typename T>
class SettingWrapper
{
public:
	using Reference_t = std::shared_ptr<Setting>;
	using Value_t = T;

public:
	explicit SettingWrapper(Reference_t ref): reference_(ref)
	{
		if (ref)
			ref->SetFormat(GetDefaultSchemaFormat<T>());
	}

	virtual ~SettingWrapper() = default;

public:
	bool has_reference() const {return reference_ != nullptr;}

	/** Returns reference to pointed-to Setting
	 * @exception NullSettingError reference is nullptr
	 * @warning Don't change the type of the result k?
	 */
	Setting& GetSetting()
	{
		if (!has_reference())
			throw NullSettingError("NullSettingError");
		return *reference_;
	}

	const Setting& GetSetting() const
	{
		return const_cast<SettingWrapper*>(this)->GetSetting();
	}

	/** Returns current stored values
	 * @exception NullSettingError reference is nullptr
	 * @see Setting
	 */
	Value_t GetValue() const {return GetSetting().template get_value<T>();}
	Value_t GetDefault() const {return GetSetting().template get_default<T>();}
	Value_t GetValueOrDefault() const
	{
		return GetSetting().template GetValueOrDefault<T>();
	}

	/** Sets current stored values
	 * @exception NullSettingError reference is nullptr
	 * @see Setting
	 */
	void SetValue(Value_t val) {GetSetting().SetValue(val);}
	void SetDefault(Value_t val) {GetSetting().SetDefault(val);}

public:
	/// Dereferences to internal reference
	Setting *operator-> () {return reference_.get();}
	const Setting *operator-> () const {return reference_.get();}

private:
	Reference_t reference_;
};


template<typename T>
constexpr Node::SchemaFormat_t GetDefaultSchemaFormat()
{
	return Node::SchemaFormat_t::number;
}

template<>
constexpr Node::SchemaFormat_t GetDefaultSchemaFormat<std::string>()
{
	return Node::SchemaFormat_t::text;
}

template<>
constexpr Node::SchemaFormat_t GetDefaultSchemaFormat<char *>()
{
	return Node::SchemaFormat_t::text;
}

template<>
constexpr Node::SchemaFormat_t GetDefaultSchemaFormat<const char *>()
{
	return Node::SchemaFormat_t::text;
}

template<>
constexpr Node::SchemaFormat_t GetDefaultSchemaFormat<bool>()
{
	return Node::SchemaFormat_t::checkbox;
}

}  // namespace dman

#endif  // SRC_CONFIG_SETTINGWRAPPER_H_
