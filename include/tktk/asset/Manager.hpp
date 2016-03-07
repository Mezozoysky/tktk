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


#ifndef TKTK_ASSET_MANAGER_HPP
#define TKTK_ASSET_MANAGER_HPP

#include <tktk/asset/Config.hpp>
#include <tktk/util/MemoryPool.hpp>
#include <unordered_map>
#include <memory>
#include <limits>

namespace tktk
{
namespace asset
{

/// \brief Marker "interface" for storing managers with std containers
class Manager
{
    Manager( const Manager& ) = delete;
    Manager& operator= ( const Manager& ) = delete;

public:
    /// \brief Invalide value for size_t indices
    const std::size_t INVALID_INDEX{ std::numeric_limits< std::size_t >::max() };

    /// \brief Default constructor
    Manager() noexcept = default;
    /// \brief Virtual destructor for further inheritance
    virtual ~Manager() noexcept = default;

    virtual std::size_t getSize() const noexcept = 0;
    virtual std::size_t getFreeSize() const noexcept = 0;

    virtual bool isIndexFree( std::size_t index ) const noexcept = 0;
    virtual void clear() noexcept = 0;

//    virtual bool exists( const std::string& string  ) const noexcept = 0;
};

//Forward declarations
class System;

/// \brief Represents the storage/management for assets
///
/// Description stub
template < typename AssetT >
class Mgr
: public Manager
{
public:
    /// \brief Wraps the managing assets actual type
    using AssetTypeT = AssetT;
    /// \brief Alias for shared asset type
    using SharedAssetTypeT = std::shared_ptr< AssetTypeT >;

    /// \brief Default constructor
    ///
    /// Does nothing
    Mgr( System* systemPtr ) noexcept
    : Manager()
    , mSystemPtr{ systemPtr }
    {
    }

    /// \brief Virtual destructor for further inheritance
    ///
    /// Does nothing
    virtual ~Mgr() noexcept
    {
    }

/*
    /// \brief Adds an asset with constructructor arguments \em args and returns an asset id
    template< typename... ArgsT >
    SharedAssetTypeT add( const std::string& name, ArgsT&&... args ) noexcept
    {
        std::uint32_t index;

        if ( mFreeIndices.empty() )
        {
            index = mSharedAssets.size();

            mSharedAssets.push_back( nullptr );
            mNames.push_back( "" );
        }
        else
        {
            index = mFreeIndices.back();
            mFreeIndices.pop_back();
        }

        SharedAssetTypeT shared{ std::make_shared< AssetTypeT >( std::forward< Args >( args )... ) };
        mSharedAssets[ index ] = shared;
        mNames[ index ] = name;
        mIndicesByNames[ name ] = index;
        return ( shared );
    }
*/
    inline std::size_t getIndex( const std::string& name ) const noexcept
    {
        std::size_t index{ INVALID_INDEX };

        auto it = mIndicesByNames.find( name );
        if ( it != mIndicesByNames.end() ) // Asset with the same name alreary exists
        {
            index = it->second; // Just give away it's index
        }

        return ( index );
    }


    std::size_t add( const std::string& name ) noexcept
    {
        std::size_t index{ getIndex( name ) };

        if ( index == INVALID_INDEX )
        {
            if ( mFreeIndices.empty() )
            {
                std::size_t size = getSize();
                index = size++;

                mSharedAssets.resize( size );
                mNames.resize( size );
            }
            else
            {
                index = mFreeIndices.back();
                mFreeIndices.pop_back();
            }

            mSharedAssets[ index ] = nullptr;
            mNames[ index ] = name;
        }

        return ( index );
    }

    /// \brief Override me!
    virtual SharedAssetTypeT loadAsset( const std::string& name ) noexcept // TODO: Should be pure virtual
    {
        SharedAssetTypeT shared;
        return ( shared );
    }

    SharedAssetTypeT load( const std::string& name, bool reload = false ) noexcept
    {
        std::size_t index{ add( name ) };
        if ( mSharedAssets[ index ] == nullptr || reload )
        {
            mSharedAssets[ index ] = loadAsset( name );
        }
        return ( mSharedAssets[ index ] );
    }

    std::size_t set( const std::string& name, SharedAssetTypeT shared ) noexcept
    {
        std::size_t index{ add( name ) };
        mSharedAssets[ index ] = shared;
        return ( index );
    }

    SharedAssetTypeT get( const std::string& name ) noexcept
    {
        std::size_t index{ add( name ) };
        SharedAssetTypeT shared = mSharedAssets[ index ];
        if ( shared == nullptr )
        {
            shared = load( name );
        }

        return ( shared );
    }

    virtual SharedAssetTypeT drop( const std::string& name ) noexcept
    {
        auto it = mIndicesByNames.find( name );
        if ( it == mIndicesByNames.end() )
        {
            return ( nullptr );
        }

        std::uint32_t index = it->second;
        SharedAssetTypeT shared{ mSharedAssets[ index ] };
        mSharedAssets[ index ] = nullptr;
        mIndicesByNames.erase( it );

        auto itFree = std::lower_bound( mFreeIndices.begin(), mFreeIndices.end(), index, std::greater< std::size_t >() );
        mFreeIndices.insert( itFree, index );
        return ( shared );
    }

    inline virtual std::size_t getSize() const noexcept override final
    {
        return ( mSharedAssets.size() );
    }

    inline virtual std::size_t getFreeSize() const noexcept override final
    {
        return ( mFreeIndices.size() );
    }

    virtual bool isIndexFree( std::size_t index ) const noexcept override final
    {
        assert( index < getSize() && "Bad index!" );
        auto it( std::find_if( mFreeIndices.begin(), mFreeIndices.end(), [index]( std::uint32_t freeIndex ) { return ( freeIndex == index ); } ) );
        return ( it != mFreeIndices.end() );
    }

    virtual void clear() noexcept override final
    {
//        for ( std::size_t i = 0; i < getSize(); ++i )
//        {
//        }

        mSharedAssets.resize( 0 );
        mNames.resize( 0 );
        mFreeIndices.resize( 0 );
        mIndicesByNames.clear();
    }

protected:
    std::vector< SharedAssetTypeT > mSharedAssets;
    std::vector< std::string > mNames;
    std::vector< std::uint32_t > mFreeIndices;
    std::unordered_map<std::string, std::uint32_t> mIndicesByNames;

    System* mSystemPtr{ nullptr };
};

} //namespace asset
} //namespace tktk

#endif /* end of include guard: TKTK_ASSET_MANAGER_HPP */
