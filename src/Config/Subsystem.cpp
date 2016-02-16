/*
    Copyright (C) 2016, EdWard <ezeward4@gmail.com>
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
#include "Subsystem.h"
#include "OrphanSubsystemError.h"

// STD Includes
#include <string>
#include <stdexcept>

namespace dman
{

void Subsystem::Register()
{
    doRegister();

    for (auto subsystem : sub_subsystems_)
    {
        subsystem.second.Register();
    }
}

bool Subsystem::Configure()
{
    bool ret = doConfigure();

    for (auto subsystem : sub_subsystems_)
    {
        if (ret &= subsystem.second.Configure())
        {
            // Failure...
        }
    }

    // set ready flag
    return ready_ = ret;
}

Subsystem::Key_t Subsystem::GetPath() const
{
    if (is_orphan())
        return get_name();
    else
        return get_parent()->GetPath() + '/' + get_name();
}

const Subsystem::SubsystemRef_t
Subsystem::GetSubSubsystem(const Key_t& key) const
{
    return sub_subsystems_.at(key);
}

bool Subsystem::IsSubSubsystem(const Key_t& key) const
{
    return sub_subsystems_.count(key) > 0;
}

Subsystem::Subsystem(Key_t name, Subsystem * parent):
    name_(name), parent_(parent), ready_(false)
{
}

Subsystem::Subsystem(Subsystem&& other):
    name_(std::move(other.name_)),
    parent_(other.parent_),
    sub_subsystems_(std::move(other.sub_subsystems_)),
    ready_(other.ready_)
{
    other.parent_ = nullptr;
    other.ready_ = false;
}

void Subsystem::SetParent(Subsystem * parent)
{
    parent_ = parent;
}

Subsystem::SubsystemRef_t Subsystem::GetSubSubsystem(const Key_t& key)
{
    return sub_subsystems_.at(key);
}

void Subsystem::AddSubSubsystem(const Key_t& key, SubsystemRef_t subsystem)
{
    // The second value of the pair returned by emplace indicates whether
    // there was indeed a value added to the map
    if (!sub_subsystems_.emplace(key, subsystem).second)
        throw std::domain_error(std::string("Subsystem: ") +
            key + " is already a subsubsystem of " + name_);

    subsystem.SetParent(this);
}

PortGroup& Subsystem::GetPortSpace(const Key_t& space)
{
    if (is_orphan())
        throw OrphanSubsystemError(get_name());

    return get_parent()->GetPortSpace(space)[get_name()];
}

SettingGroup& Subsystem::GetSettings(const Key_t& settings)
{
    if (is_orphan())
        throw OrphanSubsystemError(get_name());

    return get_parent()->GetSettings(settings)[get_name()];
}

}  // namespace dman
