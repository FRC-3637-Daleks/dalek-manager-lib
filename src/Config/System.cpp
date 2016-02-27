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
#include "System.h"
#include "OrphanSystemError.h"

// STD Includes
#include <string>
#include <stdexcept>

namespace dman
{

void System::Register()
{
    doRegister();

    for (auto subsystem : sub_subsystems_)
    {
        subsystem.second.Register();
    }

    Log(MessageData::INFO, "Register", "dman::System") << "System Registered";
}

bool System::Configure()
{
    bool ret = doConfigure();

    for (auto subsystem : sub_subsystems_)
    {
        if (!(ret &= subsystem.second.Configure()))
        {
			Log(MessageData::WARN, "Configure", "dman::System") <<
			"Failed to Configure Subsystem: " << subsystem.second.GetPath();
        }
    }

	if (ret)
		Log(MessageData::STATUS, "Configure", "dman::System") <<
			"Configured successfully. System is ready";
	else
		Log(MessageData::WARN, "Configure", "dman::System") <<
			"There were errors configuring";

    // set ready flag
    return ready_ = ret;
}

System::Key_t System::GetPath() const
{
    if (is_orphan())
        return get_name();
    else
        return get_parent()->GetPath() + '/' + get_name();
}

const System::SystemRef_t
System::GetSubSystem(const Key_t& key) const
{
    return sub_subsystems_.at(key);
}

bool System::IsSubSystem(const Key_t& key) const
{
    return sub_subsystems_.count(key) > 0;
}

System::System(Key_t name, System * parent):
	LogObject(SystemData(name, "", "")),
    name_(name), parent_(parent), ready_(false)
{
	SetParent(parent);
}

System::System(System&& other):
	LogObject(other),
    name_(std::move(other.name_)),
    parent_(other.parent_),
    sub_subsystems_(std::move(other.sub_subsystems_)),
    ready_(other.ready_)
{
    other.parent_ = nullptr;
    other.ready_ = false;
}

void System::SetParent(System * parent)
{
    parent_ = parent;
	SetSystemName(GetPath());
}

System::SystemRef_t System::GetSubSystem(const Key_t& key)
{
    return sub_subsystems_.at(key);
}

void System::AddSubSystem(const Key_t& key, SystemRef_t subsystem)
{
    // The second value of the pair returned by emplace indicates whether
    // there was indeed a value added to the map
    if (!sub_subsystems_.emplace(key, subsystem).second)
        throw std::domain_error(std::string("System: ") +
            key + " is already a subsubsystem of " + name_);

    subsystem.SetParent(this);
}

PortGroup& System::GetPortSpace(const Key_t& space)
{
    if (is_orphan())
        throw OrphanSystemError(get_name());

    return get_parent()->GetPortSpace(space)[get_name()];
}

SettingGroup& System::GetSettings(const Key_t& settings)
{
    if (is_orphan())
        throw OrphanSystemError(get_name());

    return get_parent()->GetSettings(settings)[get_name()];
}

ValueStore& System::GetValueStore(const Key_t& store_name)
{
    if (is_orphan())
        throw OrphanSystemError(get_name());

    return get_parent()->GetValueStore(store_name);
}

UpdateStore& System::GetUpdateStore(const Key_t& store_name)
{
    if (is_orphan())
        throw OrphanSystemError(get_name());

    return get_parent()->GetUpdateStore(store_name);
}

UpdateThread& System::GetUpdateThread(const Key_t& thread_name)
{
    if (is_orphan())
        throw OrphanSystemError(get_name());

    return get_parent()->GetUpdateThread(thread_name);
}




}  // namespace dman
