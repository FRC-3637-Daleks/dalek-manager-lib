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

#ifndef SRC_CONFIG_SYSTEM_H_
#define SRC_CONFIG_SYSTEM_H_

// Project Includes
#include "PortGroup.h"
#include "SettingGroup.h"
#include "TreeNode.h"
#include "OrphanSystemError.h"

#include "Utility/ValueStore.h"
#include "Utility/UpdateStore.h"
#include "Utility/UpdateThread.h"

// STD Includes
#include <string>
#include <map>


namespace dman
{

class System
{
public:
    using Key_t = TreeNode::Key_t;
    using Base_t = MapNode<System>;
    using SystemRef_t = System&;
    using SubSystems_t = std::map<Key_t, SystemRef_t>;

public:
    virtual ~System() = default;

    /// Can't copy subsystems
    System(const System&) = delete;

public:
    /** Registers all values being used and published by this subsystem by \\
     * calling \c doRegister()
     * Calls Register on all subsubsystems as well
     */
    void Register();

    /** Configures the subsystem so that it's ready to be used
     * Also calls configure on all subsubsystems
     * If the \c doConfigure function as well as all subsubsystems succeeded \\
     * the \c ready flag will be set to true
     * @return Whether the system is ready upon completeion
     */
    bool Configure();

public:
    /// Returns name. Must be defined at construction time.
    Key_t get_name() const {return name_;}

    Key_t GetPath() const;

    /// Returns reference to parent
    const System * get_parent() const {return parent_;}

    /// Returns true if this subsystem has no parent
    bool is_orphan() const {return get_parent() == nullptr;}

    /// Returns true if the subsystem successfully configured
    bool is_ready() const {return ready_;}

    /** Returns Sub-System at \c key
     * @exception std::out_of_range If key is not a subsubsystem of this object
     */
    const SystemRef_t GetSubSystem(const Key_t& key) const;

    /// Returns true if a subsystem by that key exists
    bool IsSubSystem(const Key_t& key) const;

protected:
    /** Constructs subsystem by name.
     * Protected because non-derived Systems are not allowed
     */
    System(Key_t name, System * parent = nullptr);

    /** Can move subsystems
     * @post \code{.cpp} other.name_ == "" && other.parent_ == nullptr &&
     * sub_subsystems_.empty() \endcode
     */
    System(System&& other);

protected:
    /// Returns mutable reference to parent
    System * get_parent() {return parent_;}

    /// Sets parent of this subsystem to \c parent
    void SetParent(System * parent);

    /** Returns mutable reference to Sub System at \c key
     * @exception std::out_of_range If \c key is not a subsubsystem of \\
     * this object
     */
    SystemRef_t GetSubSystem(const Key_t& key);

    /** Adds \c subsystem to Sub-Systems at \c key
     * @exception std::domain_error If \c key is already a subsubsystem of this object
     */
    void AddSubSystem(const Key_t& key, SystemRef_t subsystem);

protected:
    /** Override this function to set up config and runtime values used by
     * this system
     */
    virtual void doRegister() {}

    /** Override this function to (re)configure all components of the subsystem
     * @return true if the subsystem is ready
     */
    virtual bool doConfigure() {return true;}

protected:
    /** Returns the ports subnode at \c space /path/to/subsystem
     * This is done by recursively calling the parent's \c GetPortSpace()
     * If this function returns a reference it must be guaranteed to not be \\
     * deleted
     * @exception <unknown> throws if the \c parent_->GetPortSpace throws
     * @exception OrphanSystemError is thrown if this subsystem is an orphan
     */
    virtual PortGroup& GetPortSpace(const Key_t& space);

    /** Returns the settings subnode at \c path/to/subsystem
     * This is done by recursively calling the parent's \c GetPortSpace()
     * @param settings Name of settings to return. \\
     * Defaults to blank, indicating just the regular settings of the subsystem
     * @exception <unknown> throws if the \c parent_->GetPortSpace throws
     * @exception OrphanSystemError is thrown if this subsystem is an orphan
     */
    virtual SettingGroup& GetSettings(const Key_t& settings = "settings");

protected:
    virtual ValueStore& GetValueStore(const Key_t& store_name = "defualt");
    virtual UpdateStore& GetUpdateStore(const Key_t& store_name = "default");
    virtual UpdateThread& GetUpdateThread(
        const Key_t& thread_name = "default");

    template<typename T>
    ValueStore::Value<T> GetLocalValue(Key_t key,
                                        const Key_t& store_name = "default")
    {
        return GetValueStore(store_name).Get<T>(getLocalValueName(key));
    }

private:
    virtual Key_t getLocalValueName(Key_t local_name)
    {
        if (is_orphan())
            throw OrphanSystemError(get_name());

        return get_parent()->getLocalValueName(
            get_name() + '/' + std::move(local_name));
    }

private:
    Key_t name_;
    System *parent_;
    SubSystems_t sub_subsystems_;
    bool ready_;
};


}  // namespace dman


#endif  // SRC_CONFIG_SYSTEM_H_
