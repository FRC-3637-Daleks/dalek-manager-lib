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

#ifndef SRC_CONFIG_ROOTSYSTEM_H_
#define SRC_CONFIG_ROOTSYSTEM_H_

// Project Includes
#include "System.h"
#include "ConfigContext.h"

// STD Includes
#include <memory>

namespace dman
{

class RootSystem: public System
{
public:
    virtual ~RootSystem() = default;

public:
    /// Returns ConfigContext object
    ConfigContext& get_context() {return context_;}

protected:
    RootSystem(std::string config_home, Key_t name = "Robot");

protected:
    /** Returns the context's PortGroup at space
     */
    PortGroup& GetPortSpace(const Key_t& space) override;

    /** Returns the context's SettingGroup at settings
     * Blank is the default and will just get the standard settings
     */
    SettingGroup& GetSettings(const Key_t& settings = "settings") override;

protected:
    /** Assembles the current configuration
     */
    void doRegister() override;

    /** Loads the current configuration
     */
    bool doConfigure() override;


private:
    ConfigContext context_;
};

}  // namespace dman


#endif  // SRC_CONFIG_ROOTSYSTEM_H_
