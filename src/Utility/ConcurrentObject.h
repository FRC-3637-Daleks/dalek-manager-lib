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

#ifndef SRC_UTILITY_CONCURRENTOBJECT_H_
#define SRC_UTILITY_CONCURRENTOBJECT_H_

// STD Includes
#include <atomic>
#include <utility>
#include <mutex>
#include <type_traits>

namespace dman
{

/** Generic class for a threadsafe object/value.
 * Provides store/retrieve functions which use mutex locks
 * to prevent data races
 */
template<typename T, bool trivial = false>
class BasicConcurrentObject
{
public:
	static constexpr bool is_atomic = trivial;

public:
	BasicConcurrentObject(T&& val):
		value_(std::forward<T>(val)) {}
	BasicConcurrentObject(const BasicConcurrentObject<T, trivial>&) = default;
	BasicConcurrentObject(BasicConcurrentObject<T, trivial>&&) = default;
	~BasicConcurrentObject() = default;

public:
	/// Returns current value. Blocks until value is available
	T Retrieve() const
	{
		std::lock_guard< Mutex_t > lock(mutex_);
		return value_;
	}

	/** Sets the value at \c store to the current value if available
	 * @param store Pointer to value to store output
	 * @return true if the operation was blocked or store was nullptr
	 * False otherwise.
	 */
	bool TryRetrieve(T* const store) const
	{
		if(store == nullptr)
			return true;

		if(!mutex_.try_lock())
			return true;

		std::lock_guard< Mutex_t > lock(mutex_);
		*store = value_;
		return false;
	}

	/// Sets current value to \c val. Blocks until value is available
	void Store(T&& val)
	{
		std::lock_guard< Mutex_t > lock(mutex_);
		value_ = std::forward<T>(val);
	}

	/** Attempts to set current value to \c val if available
	 * #@return true if the operation was blocked. False otherwise.
	 */
	bool TryStore(T&& val)
	{
		if(!mutex_.try_lock())
			return true;

		Store(std::forward<T>(val));
		return false;
	}

public:
	BasicConcurrentObject& operator=(BasicConcurrentObject&&) = default;
	BasicConcurrentObject& operator=(const BasicConcurrentObject&) = default;
	BasicConcurrentObject& operator=(T&& val)
	{
		Store(std::forward<T>(val));
		return *this;
	}

	/// Returns the stored value
	operator T() const {return Retrieve();}

private:
	using Mutex_t = std::mutex;

private:
	T value_;
	mutable Mutex_t mutex_;
};

/** Specialization of BasicCurrentObject for trivially copyable types
 * Just an alias for an atomic
 */
template<typename T>
class BasicConcurrentObject<T, true>: public std::atomic<T>
{
public:
	static constexpr bool is_atomic = true;

public:
	/// Inherit Constructors
	using std::atomic<T>::atomic;

	/// Inherit assignment operators
	using std::atomic<T>::operator=;

public:
	T Retrieve() const {return this->load();}
	bool TryRetrieve(T* const store) const
	{
		if(store == nullptr)
			return true;
		*store = this->load();
		return false;
	}

	void Store(T&& val) {this->store(std::forward<T>(val));}
	bool TryStore(T&& val)
	{
		this->store(std::forward<T>(val));
		return false;
	}
};

/** Alias for a concurrent object.
 * If T is trivially copyable, the object is implemented as an atomic,
 * otherwise, it is implemented through mutex-locked accessor methods.
 */
template<typename T>
using ConcurrentObject =
	BasicConcurrentObject<T, std::is_constructible<T>::value>;

}  // namespace dman

#endif  // SRC_UTILITY_CONCURRENTOBJECT_H_
