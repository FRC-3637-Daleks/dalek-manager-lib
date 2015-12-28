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

#ifndef SRC_LOG_STREAMHANDLE_H_
#define SRC_LOG_STREAMHANDLE_H_

// STD Includes
#include <functional>
#include <string>
#include <sstream>
#include <memory>

namespace dman
{

/** Provides a "operator<<" stream interface which "flushes" on destruction.
 * This allows functions to return operator<< interfaces which have custom behavior
 * on flush.
 * \see TextLog::Log
 */
class StreamHandle
{
public:
	using Flush_func_t = std::function<void(std::string)>;

public:
	/// Constructs new StreamHandle from a flush function
	explicit StreamHandle(Flush_func_t fn):
		fn_(std::move(fn)),
		buffer_(std::make_unique<Buffer_t>()) {}

	/// Copying is disallowed as each flush can only occur once
	StreamHandle(const StreamHandle&) = delete;

	/** Move constructor moves flush function.
	 * It also prevents the flush function from executing in sh
	 */
	StreamHandle(StreamHandle&& sh) = default;

	/// Copy assignment disallowed as each flush can only occur once
	StreamHandle& operator= (const StreamHandle&) = delete;

	/** Object flushes previous contents and assigns itself contents of other.
	 * Theoretically the only way to pass in other is via move constructor,
	 * which is why this is valid
	 */
	StreamHandle& operator= (StreamHandle other);

	/// Object calls the flush function on destruction
	~StreamHandle();

public:
	/** Insert rhs into buffer
	 * @exception std::runtime_error The StreamHandle was already flushed
	 */
	template<typename T>
	StreamHandle&& operator<< (T&& rhs)
	{
		GetBuffer() << std::forward<T>(rhs);
		return std::move(*this);
	}

	/** Manually flush the contents of the buffer
	 * @exception std::runtime_error The StreamHandle was already flushed
	 */
	void Flush();

	/// Return current content of stream
	std::string GetCurrentOutput() const
	{
		return std::move(GetBuffer().str());
	}

private:
	using Buffer_t = std::ostringstream;

private:
	/// Returns internal reference to the output stream
	Buffer_t& GetBuffer() const;

private:
	/// The function to call when the object is complete
	Flush_func_t fn_;

	/// Where intermediary calls to operator<< are stored
	std::unique_ptr<Buffer_t> buffer_;
};

}  // namespace dman

#endif  // SRC_LOG_STREAMHANDLE_H_
