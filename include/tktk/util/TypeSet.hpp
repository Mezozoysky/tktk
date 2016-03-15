//  tktk
//
//  tktk - Copyright (C) 2015 Stanislav Demyanovich <stan@angrybubo.com>
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
/// \brief This file provides TypeSet type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib

#ifndef TKTK_UTIL_TYPE_SET_HPP
#define TKTK_UTIL_TYPE_SET_HPP

#include <tktk/util/Config.hpp>
#include <bitset>
#include <cstddef>

namespace tktk
{
namespace util
{

template < std::size_t sizeV = 64 >
class TypeSet
{
    using SetImpl = std::bitset< sizeV >;

public:

    static const std::size_t setSizeV{ sizeV };


    TypeSet< sizeV >& set() noexcept
    {
        mSet.set();
        return ( *this );
    }

    template< typename TypeKey >
    TypeSet< sizeV >& set( bool value = true )
    {
        std::size_t pos{ getUniqueTypeId< TypeKey >() };
        mSet.set( pos, value );
        return ( *this );
    }

    TypeSet< sizeV >& reset() noexcept
    {
        mSet.reset();
        return ( *this );
    }

    template< typename TypeKey >
    TypeSet< sizeV >& reset()
    {
        std::size_t pos{ getUniqueTypeId< TypeKey >() };
        mSet.reset( pos );
        return ( *this );
    }

    TypeSet< sizeV >& flip() noexcept
    {
        mSet.flip();
        return ( *this );
    }

    template< typename TypeKey >
    TypeSet< sizeV >& flip()
    {
        std::size_t pos{ getUniqueTypeId< TypeKey >() };
        mSet.flip( pos );
        return ( *this );
    }

    template< typename TypeKey >
    bool test() const
    {
        std::size_t pos{ getUniqueTypeId< TypeKey >() };
        return ( mSet.test( pos ) );
    }

private:

    template< typename TypeKey >
    inline static std::size_t getUniqueTypeId() noexcept
    {
        static const std::size_t id = mNextTypeId++;
        return id;
    }

private:
    static std::atomic_size_t mNextTypeId;
    SetImpl mSet;
};

//
// Template members definition
//

template< std::size_t sizeV >
std::atomic_size_t TypeSet< sizeV >::mNextTypeId{ 0 };


} // namespace util
} // namespace tktk


#endif // TKTK_UTIL_TYPE_SET_HPP
