/* 
    Copyright (C) 2015 EdWard
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

#ifndef SRC_LOG_TEXTLOGLISTENER_H_
#define SRC_LOG_TEXTLOGLISTENER_H_

// Includes
#include <string>
#include "LogData.h"

namespace dman
{

/** Event Listener Interface for a Library Text Log.
 * When something is logged somewhere, the contents are dispatched to a list of derived TextLogListeners
 * Derived classes should define the actual logging behavior, and be passed into the TextLog Manager.
 */
class TextLogListener
{
public:
	virtual ~TextLogListener() {}

public:
	/** Logs a line of text
	 * @param line Formatted log string
	 */
	virtual void Log(const std::string &line) = 0;

	/** Logs components of LogData
	 * Default behavior is to pass its string representation into Log(std::string)
	 * @param log_data Object containing log info
	 */
	virtual void Log(const LogData& log_data) {Log(log_data.ToString());}
};

}  // namespace dman

#endif  // SRC_LOG_TEXTLOGLISTENER_H_
