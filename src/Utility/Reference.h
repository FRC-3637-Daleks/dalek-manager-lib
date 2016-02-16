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

#ifndef SRC_UTILITY_REFERENCE_H_
#define SRC_UTILITY_REFERENCE_H_

// Project Includes
#include "Gettable.h"
#include "ValueStore.h"

// STD Includes
#include <stdexcept>

namespace dman
{

template<typename T>
class Reference: public Gettable<T>
{
public:
    using Reference_t = ValueStore::Value<T>;
    using Key_t = ValueStore::Key_t;

public:
    Reference(Reference_t ref): ref_(std::move(ref))
    {
    }

    virtual ~Reference() = default;

public:
    /// Returns reference key
    Key_t get_reference_key() const {return ref_.get_key();}

    /// Returns copy of the internal Value
    Reference_t get_reference() const {return ref_;}

    /// Sets the internal reference
    void SetReference(Reference_t ref) {ref_ = std::move(ref);}

public:
    T Get() const override
    {
        if (ref_.initialized())
            return ref_.GetValue();
        else
            return T();
    }

private:
    Reference_t ref_;
};

template<typename T>
void ValueStore::Value<T>::SetReference(Value<T> ref)
{
    if (has_refer())
    {
        get_refer()->SetReference(std::move(ref));
    }
    else
    {
        throw std::logic_error("ValueStore::Value: " +
            get_key() + " attempt to set a reference on a Value which " +
            "does not own the Reference object");
    }
}

template<typename T>
ValueStore::Value<T> ValueStore::Bind(Key_t key, Key_t ref_key)
{
    auto ret = Get<T>(key);
    auto ref = Get<T>(ref_key);
    if (ret.initialized())
    {
        if (ret.has_refer())
        {
            ret.SetReference(ref);
        }
        else
        {
            throw std::logic_error(std::string("ValueStore: ") +
                key + " is already initialized and does not own " +
                " a reference object");
        }
    }
    else
    {
        ret.Initialize(std::make_shared<typename Value<T>::Reference_t>(ref));
    }

    return std::move(ret);
}


}  // namespace dman


#endif  // SRC_UTILITY_REFERENCE_H_
