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

#ifndef SRC_WPILIB_WPISYSTEM_H_
#define SRC_WPILIB_WPISYSTEM_H_

// STD Includes
#include <string>

// WPILib Includes
#include "WPILib.h"

// Project Includes
#include "Config/System.h"
#include "Config/RootSystem.h"

namespace dman
{

/** Wrapper for a Subsystem wich is also a WPILib Subsystem
 * @tparam SystemT class which derives \c dman::System
 */
template<class SystemT>
class SystemWithWPI: public Subsystem, public SystemT
{
public:
	using Name_t = std::string;

public:
	SystemWithWPI(const Name_t name): Subsystem(name), SystemT(name) {}
};

using WPISystem = SystemWithWPI<dman::System>;
using WPIRootSystem = SystemWithWPI<dman::RootSystem>;


}  // namespace dman

#endif  // SRC_WPILIB_WPISYSTEM_H_
