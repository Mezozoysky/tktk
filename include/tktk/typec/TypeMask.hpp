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
/// \brief This file provides TypeMask type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib

#ifndef TKTK_TYPEC_TYPE_MASK_HPP
#define TKTK_TYPEC_TYPE_MASK_HPP

#include <tktk/typec/Config.hpp>
#include <bitset>
#include <cstddef>

namespace tktk
{
namespace typec
{

template < std::size_t sizeV = 64 >
class TypeMask
{
    using SetImpl = std::bitset< sizeV >;

public:

    static const std::size_t setSizeV{ sizeV };


    TypeMask< sizeV >& set() noexcept
    {
        mSet.set();
        return ( *this );
    }

    template< typename TypeKey >
    TypeMask< sizeV >& set( bool value = true )
    {
        std::size_t pos{ getUniqueTypeId< TypeKey >() };
        mSet.set( pos, value );
        return ( *this );
    }

    TypeMask< sizeV >& reset() noexcept
    {
        mSet.reset();
        return ( *this );
    }

    template< typename TypeKey >
    TypeMask< sizeV >& reset()
    {
        std::size_t pos{ getUniqueTypeId< TypeKey >() };
        mSet.reset( pos );
        return ( *this );
    }

    TypeMask< sizeV >& flip() noexcept
    {
        mSet.flip();
        return ( *this );
    }

    template< typename TypeKey >
    TypeMask< sizeV >& flip()
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
    inline std::size_t getUniqueTypeId() const noexcept
    {
        static std::size_t nextTypeId{ 0 };
        static const std::size_t id = nextTypeId++;
        return id;
    }

private:

    SetImpl mSet;
};


} // namespace typec
} // namespace tktk


#endif // TKTK_TYPEC_TYPE_MASK_HPP
