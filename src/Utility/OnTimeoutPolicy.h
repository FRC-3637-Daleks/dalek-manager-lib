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

#ifndef SRC_UTILITY_ONTIMEOUTPOLICY_H_
#define SRC_UTILITY_ONTIMEOUTPOLICY_H_

// Project Includes
#include "Updateable.h"


// STD Includes
#include <functional>

namespace dman
{

class OnTimeoutPolicy
{
public:
    enum PolicyType {
        REMOVE = 0x1,  ///< Intended to tell UpdateStore to remove Updateable
        LOG = 0x2,  ///< Intended for caller to Log
        CUSTOM = 0x4,  ///< Indicates a custom function to call
        BAIL = 0x8  ///< Indicates UpdateStore to stop the current Update
    };

    using Custom_fn_t = std::function< void(Updateable *) >;

public:
    OnTimeoutPolicy(uint8_t policy_init,
                    Custom_fn_t custom):
        policy_(policy_init), custom_(custom) {}

    OnTimeoutPolicy(uint8_t policy_init = PolicyType::LOG):
        policy_(policy_init) {}

    OnTimeoutPolicy(const OnTimeoutPolicy&) = default;

public:
    void add_policy(uint8_t mask) {policy_ |= mask;}
    void remove_policy(uint8_t mask) {policy_ ^= policy_ & mask;}
    uint8_t get_policy() const {return policy_;}

    bool removes() const {return policy_ & PolicyType::REMOVE;}
    bool logs() const {return policy_ & PolicyType::LOG;}
    bool customs() const {return policy_ & PolicyType::CUSTOM &&
                                    custom_;}
    bool bails() const {return policy_ & PolicyType::BAIL;}

    void add_remove() {add_policy(PolicyType::REMOVE);}
    void add_log() {add_policy(PolicyType::LOG);}
    void add_bail() {add_policy(PolicyType::BAIL);}

    /// Sets custom function to call on DoAction
    void AddCustom(Custom_fn_t custom);

    /// Removes custom function to call on DoAction
    void RemoveCustom();

    /// Calls the custom function currently set
    void DoCustom(Updateable * const updater);

private:
    uint8_t policy_;
    Custom_fn_t custom_;

};

}  // namespace dman

#endif  // SRC_UTILITY_ONTIMEOUTPOLICY_H_
