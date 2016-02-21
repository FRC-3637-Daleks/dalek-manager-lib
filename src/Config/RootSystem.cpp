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

#include "RootSystem.h"

namespace dman
{

RootSystem::RootSystem(std::string config_home, Key_t name):
    Subsystem(std::move(name)), context_(std::move(config_home))
{
}

PortGroup& RootSystem::GetPortSpace(const Key_t& space)
{
    return context_.get_ports()[space];
}

SettingGroup& RootSystem::GetSettings(const Key_t& settings)
{
    if (settings.empty())
        return context_.get_settings();
    else
        return context_.get_settings()[settings];
}

void RootSystem::doRegister()
{
    context_.AssembleConfig();
}

bool RootSystem::doConfigure()
{
    return context_.LoadConfig();
}

}  // namespace dman
