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

#include "OnTimeoutPolicy.h"

namespace dman
{

void OnTimeoutPolicy::AddCustom(Custom_fn_t custom)
{
    add_policy(PolicyType::CUSTOM);
    custom_ = std::move(custom);
}

void OnTimeoutPolicy::RemoveCustom()
{
    custom_ = Custom_fn_t();
    remove_policy(PolicyType::CUSTOM);
}

void OnTimeoutPolicy::DoCustom(Updateable * const updater)
{
    if (updater == nullptr)
        return;

    if (customs() && updater->has_timeout())
    {
        custom_(updater);
    }
}


}  // namespace dman
