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
/// \brief This file provides asset::Manager type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib


#ifndef TKTK_ASSET_ASSET_POOL_HPP
#define TKTK_ASSET_ASSET_POOL_HPP

#include <tktk/asset/Config.hpp>
#include <tktk/asset/Asset.hpp>
#include <tktk/util/MemoryPool.hpp>
#include <unordered_map>

namespace tktk
{
namespace asset
{

/// \brief Marker "interface" for storing asset pools with std containers
class AssetPoolBase
{
public:
    /// \brief Virtual destructor for further inheritance
    virtual ~AssetPoolBase() noexcept
    {
    }
};

/// \brief Represents the storage/management for assets
///
/// Description stub
template < typename AssetT >
class AssetPool
: public AssetPoolBase
{
public:
    /// \brief Wraps the managing assets actual type
    using AssetTypeT = AssetT;
    using AssetDataTypeT = typename AssetTypeT::DataTypeT;

    /// \brief Default constructor
    ///
    /// Does nothing
    AssetPool() noexcept
    {
    }

    /// \brief Virtual destructor for further inheritance
    ///
    /// Does nothing
    virtual ~AssetPool() noexcept
    {
    }

    /// \brief Adds an asset with constructructor arguments \em args and returns an asset id
    template< typename... ArgsT >
    util::Id64 add( ArgsT&&... args ) noexcept
    {
        util::Id64 aId{ mPool.createElement( std::forward< ArgsT >( args )... ) };
        if ( mPool.isIdValid( aId ) )
        {
            mIdByNameMap.insert( { mPool[ aId.index() ].getFilename(), aId } );
        }
        return ( aId );
    }

    virtual void remove( const util::Id64& aId ) noexcept
    {
        if ( !mPool.isIdValid( aId ) )
        {
            return;
        }
        std::string filename{ mPool[ aId.index() ].getFilename() };
        mIdByNameMap.erase( filename );
        mPool.destroyElement( aId );
    }

    virtual bool isIdValid( const util::Id64& aId ) const noexcept
    {
        return ( mPool.isIdValid( aId ) );
    }

    /// \brief Returns raw pointer to the asset with id \em aId
    inline AssetTypeT* getPtr( const util::Id64& aId ) const noexcept
    {
        return ( mPool.getPtr( aId.index() ) );
    }


    virtual std::shared_ptr<AssetDataTypeT> getData( const util::Id64& aId ) const noexcept
    {
        return ( mPool[ aId.index() ].getData() );
    }

protected:
    using PoolTypeT = util::MemoryPool< AssetTypeT >;
    PoolTypeT mPool;
    using MapTypeT = std::unordered_map< std::string, util::Id64 >;
    MapTypeT mIdByNameMap;
};

} //namespace asset
} //namespace tktk

#endif /* end of include guard: TKTK_ASSET_ASSET_POOL_HPP */
