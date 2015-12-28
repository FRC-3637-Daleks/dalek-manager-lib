/*
    Copyright (C) 2015, EdWard <ezeward4@gmail.com>
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

// Project Includes
#include "StreamHandle.h"

// STD Includes
#include <string>
#include <sstream>
#include <functional>
#include <memory>
#include <stdexcept>

namespace dman
{

StreamHandle& StreamHandle::operator= (StreamHandle other)
{
	// Perform assignmnet using copy-and swap idiom
	fn_.swap(other.fn_);
	buffer_.swap(other.buffer_);

	// Other should call the Flush for *this's previous contents
	return *this;
}

StreamHandle::~StreamHandle()
{
	if(buffer_)
		Flush();
}

void StreamHandle::Flush()
{
	/// Executes the flush
	fn_(GetCurrentOutput());

	/* This will transfer ownership to a local variable 
	* which will destruct at function end
	*/
	auto bufferToDelete = std::move(buffer_);
}

StreamHandle::Buffer_t& StreamHandle::GetBuffer() const
{
	if(buffer_ == nullptr)
	{
		throw std::runtime_error(
			"StreamHandle: Illegal attempt to access internal buffer "
			"which has been flushed or is otherwise in an invalid state");
	}
	return *buffer_;
}

}  // namespace dman
