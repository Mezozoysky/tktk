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
/// \brief This file provides asset::System type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib


#ifndef TKTK_ASSET_SYSTEM_HPP
#define TKTK_ASSET_SYSTEM_HPP

#include <tktk/asset/Config.hpp>
#include <tktk/asset/Asset.hpp>
#include <tktk/asset/AssetPool.hpp>
#include <tktk/util/Id64.hpp>
#include <tktk/util/TypeMap.hpp>
#include <cassert>

namespace tktk
{
namespace asset
{

/// \brief Represents the common assets access functionality
///
/// Description stub
class System
{
public:
    /// \brief Default constructor
    ///
    /// Does nothing
    System() noexcept
    {
    }

    /// \brief Virtual destructor for further inheritance
    ///
    /// Does nothing
    virtual ~System() noexcept
    {
    }

    template < typename T >
    void regAssetType() noexcept
    {
        mPoolByAssetType.insert< T >( new AssetPool< T >() );
    }

    /// \brief Adds a new asset into the system
    template< typename T, typename... ArgsT >
    util::Id64 add( ArgsT&&... args ) noexcept
    {
        auto it( mPoolByAssetType.find< T >() );
        if ( it == mPoolByAssetType.end() )
        {
            assert( false && "Asset type T doesnt registered within the asset system" );
        }
        auto pool( static_cast< AssetPool< T >* >( it->second ) );
        auto aId( pool->add( std::forward< ArgsT >( args )... ) );

        return ( aId );
    }

    template < typename T >
    void remove( const util::Id64& aId ) noexcept
    {
        auto it( mPoolByAssetType.find< T >() );
        if ( it == mPoolByAssetType.end() )
        {
            assert( false && "Asset type T doesnt registered within the asset system" );
        }
        auto pool( static_cast< AssetPool< T >* >( it->second ) );
        pool->remove( aId );
    }

protected:
    util::TypeMap< AssetPoolBase* > mPoolByAssetType;
};

} //namespace asset
} //namespace tktk

#endif /* end of include guard: TKTK_ASSET_SYSTEM_HPP */
