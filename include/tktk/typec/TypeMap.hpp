/**
    tktk

    tktk - Copyright (C) 2015 Stanislav Demyanovich <stan@angrybubo.com>

    This software is provided 'as-is', without any express or
    implied warranty. In no event will the authors be held
    liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute
    it freely, subject to the following restrictions:

    1.  The origin of this software must not be misrepresented;
        you must not claim that you wrote the original software.
        If you use this software in a product, an acknowledgment
        in the product documentation would be appreciated but
        is not required.

    2.  Altered source versions must be plainly marked as such,
        and must not be misrepresented as being the original software.

    3.  This notice may not be removed or altered from any
        source distribution.
*/

#ifndef TKTK_TYPEC_TYPE_MAP_HPP
#define TKTK_TYPEC_TYPE_MAP_HPP

#include <tktk/typec/Config.hpp>
#include <cstdint>
#include <atomic>
#include <unordered_map>
#include <algorithm>

namespace tktk
{
namespace typec
{

template < typename ValueTypeT >
class TypeMap
{

private:
    using MapImpl = std::unordered_map< std::uint32_t, ValueTypeT >;

public:
    using Iterator = typename MapImpl::iterator;
    using ConstIterator = typename MapImpl::const_iterator;
    using ValueType = typename MapImpl::value_type;

    template< typename TypeKey >
    inline void insert( const ValueTypeT& value ) noexcept
    {
        mMap.insert( std::make_pair( getUniqueTypeId< TypeKey >(), value ) );
    }

    template< typename TypeKey >
    inline ValueTypeT& at()
    {
        return ( mMap.at( getUniqueTypeId< TypeKey >() ) );
    }

    template< typename TypeKey >
    inline const ValueTypeT& at() const
    {
        return ( mMap.at( getUniqueTypeId< TypeKey >() ) );
    }

    template< typename TypeKey >
    inline std::size_t count() const noexcept
    {
        return ( mMap.count( getUniqueTypeId< TypeKey >() ) );
    }

    template< typename TypeKey >
    inline std::size_t erase() noexcept
    {
        return ( mMap.erase( getUniqueTypeId< TypeKey >() ) );
    }

    inline Iterator erase( ConstIterator pos )
    {
        return ( mMap.erase( pos ) );
    }

    inline Iterator erase( ConstIterator first, ConstIterator last)
    {
        return ( mMap.erase( first, last ) );
    }

    inline void forEach( const std::function< void(ValueTypeT&) >& func )
    {
        std::for_each( mMap.begin(), mMap.end(), func );
    }

    inline ConstIterator begin() const noexcept
    {
        return ( mMap.begin() );
    }

    inline ConstIterator end() const noexcept
    {
        return ( mMap.end() );
    }

    inline Iterator begin() noexcept
    {
        return ( mMap.begin() );
    }

    inline Iterator end() noexcept
    {
        return ( mMap.end() );
    }

    template < typename TypeKey >
    inline Iterator find() noexcept
    {
        return ( mMap.find( getUniqueTypeId< TypeKey >() ) );
    }

    template < typename TypeKey >
    inline ConstIterator find() const noexcept
    {
        return ( mMap.find( getUniqueTypeId< TypeKey >() ) );
    }

    inline void clear() noexcept
    {
        mMap.clear();
    }

    inline std::size_t size() const noexcept
    {
        return ( mMap.size() );
    }

private:

    template< typename TypeKey >
    inline static int getUniqueTypeId() noexcept
    {
        static const std::size_t id = mNextTypeId++;
        return id;
    }

private:
    static std::atomic_size_t mNextTypeId;
    MapImpl mMap;
};

template< typename ValueTypeT >
std::atomic_size_t TypeMap< ValueTypeT >::mNextTypeId{ 0 };


} //namespace typec
} //namespace tktk

#endif /* end of include guard: TKTK_TYPEC_TYPE_MAP_HPP */
