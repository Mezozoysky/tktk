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
/// \brief This file provides TypeMap type (associative container with c++ types as keys)
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib

/// \namespace typemap
/// \brief Namespace for all the names of tktk-typemap library
///
/// Description stub for `namespace typemap`

#ifndef TKTK_TYPEMAP_TYPEMAP_HPP
#define TKTK_TYPEMAP_TYPEMAP_HPP


#include <tktk/typemap/Config.hpp>
#include <vector>
#include <cstddef>
#include <atomic>
#include <algorithm>
#include <functional>

namespace tktk
{
namespace typemap
{

template < typename ValueT >
class TypeMap
{
public:
    using ValueTypeT = ValueT;

    TypeMap( const ValueTypeT& invalidValue ) noexcept
    : mInvalidValue{ invalidValue }
    {
    }

    template< typename KeyT >
    void set( const ValueTypeT& value ) noexcept
    {
        std::size_t index{ getUniqueTypeId< KeyT >() };
        if ( index == mTypes.size() )
        {
            mTypes.push_back( value );
            ++mElementsCount;
        }
        else
        {
            if ( mTypes[ index ] == mInvalidValue && value != mInvalidValue )
            {
                ++mElementsCount;
            }
            mTypes[ index ] = value;
        }
    }

    template< typename KeyT >
    void erase() noexcept
    {
        std::size_t index{ getUniqueTypeId< KeyT >() };
        if ( index == mTypes.size() )
        {
            mTypes.push_back( mInvalidValue );
        }
        else
        {
            if ( mTypes[ index ] != mInvalidValue )
            {
                mTypes[ index ] = mInvalidValue;
                --mElementsCount;
            }
        }
    }

    template< typename KeyT >
    ValueTypeT& get() noexcept
    {
        std::size_t index{ getUniqueTypeId< KeyT >() };
        if ( index == mTypes.size() )
        {
            mTypes.push_back( mInvalidValue );
        }

        return ( mTypes[ index ] );
    }

    //template < typename FuncT >
    inline void forEach( const std::function< void(ValueTypeT&) >& func )
    {
        std::for_each( mTypes.begin(), mTypes.end(), func );
    }

    inline const ValueTypeT& getInvalid() const noexcept
    {
        return ( mInvalidValue );
    }

    void clear() noexcept
    {
        for ( std::size_t index{ 0 }; index < mTypes.size(); ++index )
        {
            if ( mTypes[ index ] != mInvalidValue )
            {
                mTypes[ index ] = mInvalidValue;
            }
        }
        mElementsCount = 0;
    }

    inline std::size_t getSize() const noexcept // Size of underlaying container ( number of all type were added)
    {
        return ( mTypes.size() );
    }

    inline std::size_t getElementsCount() const noexcept // Number of elements are currently set
    {
        return ( mElementsCount );
    }

private:

    template< typename KeyT >
    inline static std::size_t getUniqueTypeId() noexcept
    {
        static const std::size_t id = mNextTypeId++;
        return id;
    }

private:
    static std::atomic_int mNextTypeId;
    std::vector< ValueTypeT > mTypes;
    std::size_t mElementsCount{ 0 };
    ValueTypeT mInvalidValue; // For pointers, smart-pointers and tktk-ecs ids/handles would be invalid out of the box
};

template< typename ValueT >
std::atomic_int TypeMap< ValueT>::mNextTypeId{ 0 };


} //namespace util
} //namespace tktk


#endif // TKTK_TYPEMAP_CONFIG_HPP
