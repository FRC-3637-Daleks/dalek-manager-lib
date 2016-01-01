/*
    Copyright (C) 2015 EdWard <ezeward4@gmail.com>
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

#ifndef SRC_UTILITY_VALUESTORE_H_
#define SRC_UTILITY_VALUESTORE_H_

// Project Includes
#include "Valuable.h"
#include "Settable.h"
#include "SetValue.h"

// Boost Includes
#include <boost/any.hpp>

// STD Includes
#include <string>
#include <map>
#include <memory>
#include <stdexcept>

namespace dman
{

/** Manages syncronized map of strings to Valuables
 * Returns handles to internal Valuables of the map. The referred to 
 * Valuable within one of these is consistent across all others.
 * This allows any Value handle to initialize the value, and others will
 * have access to the Get/Read interface of this value.
 */
class ValueStore
{
public:
	using Key_t = const std::string;
	using Internal_t = boost::any;
	using Map_t = std::map<Key_t, Internal_t>;

	template<typename T>
	class Value;

private:
	template<typename T>
	struct ValueData
	{
		ValueData(Key_t key,
				  typename Value<T>::Mapped_t value_ref,
				  typename Value<T>::Setter_t *setter = nullptr):
				  key_(key), value_ref_(value_ref), setter_(setter) {}
		Key_t key_;
		typename Value<T>::Mapped_t value_ref_;
		typename Value<T>::Setter_t *setter_{nullptr};
	};

public:
	/** This is a handle for elements of ValueStore.
	 * ValueStore actually holds pointers to elements, which are pointed
	 * to the Valuable which is used to initialize a Value corresponding to 
	 * that key. This allows all parts of the program to get constant time 
	 * access to the mapped value by requesting a handle early. One and only
	 * one Value must Initialize for each mapped value.
	 */
	template<typename T>
	class Value
	{
	public:
		using Stored_t = const Valuable<T> *;
		using Mapped_t = std::shared_ptr<Stored_t>;
		using Setter_t = Settable<T>;
		using Setter_ref_t = Setter_t *;

	public:
		explicit Value(ValueData<T> vd): data_(std::move(vd)) {}
		Value(const Value&) = default;
		Value(Value&&) = default;
		Value<T>& operator= (const Value<T>&) = default;
		Value<T>& operator= (Value<T>&&) = default;

	public:
		/// Returns key the Value was requested from
		Key_t get_key() const {return data_.key_;}

		/** Returns true if this Value is initialized.
		 * This is done via a call to Initialize.
		 */
		bool initialized() const {return (*data_.value_ref_) != nullptr;}

		/** Returns true if \code{.cpp} initialized() == true \endcode
		 * and this instance was the intializer, and it was initialized with
		 * a SetValue.
		 */
		bool can_set() const {return data_.setter_ != nullptr;}

		/** Returns the value of the internal Valuable
		 * @pre \code{.cpp} initialized() == true \endcode
		 * @exception std::logic_error If preconditions aren't met
		 */
		T GetValue() const
		{
			if(!initialized())
				throw std::logic_error(
					std::string("ValueStore::Value(") + get_key() +
					") was not initialized before GetValue was called");
			return GetValuable().Get();
		}

		/** Sets the internal value to val
		 * @pre \code{.cpp} can_set() == true \endcode
		 * @exception std::logic_error Message which precondition wasn't met
		 */
		void SetValue(T&& val)
		{
			if(!initialized())
				throw std::logic_error(
					std::string("ValueStore::Value(") + get_key() +
					") was not initialized before SetValue was called");
			else if (!can_set())
				throw std::logic_error(
					std::string("ValueStore::Value(") + get_key() +
					") doesn't own a setter object and SetValue was called");

			data_.setter_->Set(std::forward<T>(val));
		}

		/** Initializes the internal Valuable to obj
		 * @pre \code{.cpp} initialized() == false \endcode
		 * @exception std::logic_error Precondition wasn't met
		 */
		void Initialize(Stored_t obj)
		{
			if(initialized())
				throw std::logic_error(
					std::string("ValueStore::Value(") + get_key() +
					"): an Initialize was attempted but this object"
					" was already Initialized");

			*data_.value_ref_ = obj;
		}

		/** Initializes the internal Valuable to obj and sets the setter to it
		 * @param obj an object of or deriving SetValue<T>
		 * @pre \code{.cpp} initialized() == false \endcode
		 * @exception std::logic_error Precondition wasn't met
		 */
		void Initialize(dman::SetValue<T> * obj)
		{
			Initialize(static_cast<Stored_t>(obj));
			data_.setter_ = obj;
		}

	private:
		const Valuable<T>& GetValuable() const
		{
			if(!initialized())
				throw std::logic_error(
					std::string("ValueStore::Value(") + get_key() +
					"): an attempt was made to access a value that wasn't"
					" allocated, i.e. initialized via Value<T>::Initialize");

			return **data_.value_ref_;
		}

	private:
		ValueData<T> data_;
	};

public:
	/** Constructs and returns a Value<T> which refers to \c key
	 * If there were no element of key previously, one will be allocated
	 */
	template<typename T>
	Value<T> Get(Key_t key)
	{
		using Mapped_t = typename Value<T>::Mapped_t;
		using Stored_t = typename Value<T>::Stored_t;

		Internal_t& elem = map_[key];
		if(elem.empty())
			elem = Mapped_t();

		Mapped_t& elemContain =
			boost::any_cast< Mapped_t& >(elem);

		if(elemContain == nullptr)
		{
			// Constructs a shared_ptr pointing to a pointer to nullptr
			elemContain = std::move(std::make_shared< Stored_t >(nullptr));
		}

		return Value<T>(ValueData<T>(key, elemContain, nullptr));
	}

	/** Gets, initializes, and returns a Value<T> which refers to \c key
	 * @param key Key returned Value will refer to
	 * @param args Arguments with which to initialize the Value<T>
	 * @pre \code{.cpp} Get<T>(key).initialized() == false \endcode
	 * @see ValueStore::Value<T>::Initialize
	 */
	template<typename T, typename... Args>
	Value<T> Initialize(Key_t key, Args&&... args)
	{
		auto ret = Get<T>(key);
		ret.Initialize(std::forward<Args>(args)...);
		return ret;
	}

private:
	Map_t map_;
};

}  // namespace dman

#endif  // SRC_UTILITY_VALUESTORE_H_
