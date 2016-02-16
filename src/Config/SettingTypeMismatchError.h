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

#ifndef SRC_CONFIG_SETTINGTYPEMISMATCHERROR_H_
#define SRC_CONFIG_SETTINGTYPEMISMATCHERROR_H_

// Project Includes
#include "Setting.h"

// STD Includes
#include <stdexcept>
#include <string>

namespace dman
{

/** Exception which indicates that there was an attempt to change \\
 * the type of a \c Setting
 */
class SettingTypeMismatchError: public std::domain_error
{
public:
    /** Constructor
     * @param required type of the Setting
     * @param used type attempted to be set
     */
    SettingTypeMismatchError(json::value_t required,
                             json::value_t used):
        std::domain_error(std::string("Mismatched type: ") +
            Setting::JSONTypeToString(required) + " required. " +
            Setting::JSONTypeToString(used) + " used.")
        {
        }
};


}  // namespace dman


#endif  // SRC_CONFIG_SETTINGTYPEMISMATCHERROR_H_
