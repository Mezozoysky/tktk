//  tktk
//
//  tktk - Copyright (C) 2016 Stanislav Demyanovich <stan@angrybubo.com>
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
/// \brief This file provides base Mgr template
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib


#ifndef TKTK_ECS_MGR_HPP
#define TKTK_ECS_MGR_HPP

#include <tktk/ecs/Config.hpp>

namespace tktk
{
namespace ecs
{

// Forward declarations
// class SystemB;

/// \brief Marker "interface" for storing managers with std containers
class Manager
{
    Manager( const Manager& ) = delete;
    Manager& operator= ( const Manager& ) = delete;

public:
    /// \brief Default constructor
    Manager() noexcept = default;
    /// \brief Virtual destructor for further inheritance
    virtual ~Manager() noexcept = default;
};

template < typename DataT >
class Mgr
: public Manager
{
public:
    /// \brief Wraps the managing data type
    using DataTypeT = DataT;

    /// \brief Default constructor
    ///
    /// Does nothing
    Mgr() noexcept
    : Manager()
    {
    }

    /// \brief Virtual destructor for further inheritance
    ///
    /// Does nothing
    virtual ~Mgr() noexcept
    {
    }
};



} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_MGR_HPP */
