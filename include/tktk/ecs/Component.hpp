//  tktk
//
//  tktk - Copyright (C) 2015-2016 Stanislav Demyanovich <stan@angrybubo.com>
//
//  This software is provided 'as-is', without any express or
//  implied warranty. In no event will the authors be held
//  liable for any damages arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute
//  it freely, subject to the following restrictions:
//
//  1.  The origin of this software must not be misrepresented;
//      you must not claim that you wrote the original software.
//      If you use this software in a product, an acknowledgment
//      in the product documentation would be appreciated but
//      is not required.
//
//  2.  Altered source versions must be plainly marked as such,
//      and must not be misrepresented as being the original software.
//
//  3.  This notice may not be removed or altered from any
//      source distribution.



/// \file
/// \brief This file provides 'component' of 'entity-component system'
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2015-2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib

#ifndef TKTK_ECS_COMPONENT_HPP
#define TKTK_ECS_COMPONENT_HPP

#include <tktk/ecs/Config.hpp>
#include <tktk/mpool/Id64.hpp>
#include <memory>
#include <string>

namespace tktk
{
namespace ecs
{

/// \brief Plain base class for Comp<CompT> template
struct Component
{
    /// \brief Explitit constructor from entity id
    explicit Component( const mpool::Id64& entityId ) noexcept;

    /// \brief Virtual destructor for further inheritance
    //virtual ~Component() noexcept;

    /// \brief Gets the owner entity id
    inline const mpool::Id64& getEntityId() const noexcept
    {
        return mEntityId;
    }

private:
    /// \brief Owner entity id
    mpool::Id64 mEntityId;
};

/// \brief Base class for all component You want
template< typename CompT >
struct Comp
: public Component
{
    /// \brief Wrapper for derived component type
    using CompTypeT = CompT;
    /// \brief Wrapper for base component type
    using BaseTypeT = Comp< CompTypeT >;

    /// \brief Excplicit constructor from entity id
    explicit Comp( const mpool::Id64& entityId ) noexcept;

    /// \brief Virtual destructor for further inheritance
//     virtual ~Comp() noexcept;
};

//
// Template-methods definitions
//

// Comp<CompT>

template < typename CompT >
Comp< CompT >::Comp( const mpool::Id64& entityId ) noexcept
: Component{ entityId }
{
}

// template < typename CompT >
// Comp< CompT >::~Comp() noexcept
// {
// }


} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_COMPONENT_HPP */
