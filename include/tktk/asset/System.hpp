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
#include <tktk/asset/Manager.hpp>
#include <tktk/util/TypeMap.hpp>
#include <cassert>
#include <type_traits>

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
        // Unregister and destroy processors
        auto it( mManagers.begin() );
        while ( it != mManagers.end() )
        {
            auto mgrPtr( it->second );
            it = mManagers.erase( it );
            delete mgrPtr;
            mgrPtr = nullptr;
        }
    }

    template < typename MgrT, typename... ArgsT >
    MgrT* registerMgr( ArgsT&&... args ) noexcept
    {
        static_assert(
            std::is_base_of< Mgr< typename MgrT::AssetTypeT >, MgrT >::value
            , "MgrT should extend tktk::asset::Mgr<>"
        );

        if ( mManagers.count< typename MgrT::AssetTypeT >() == 1 )
        {
            ll_error( "Trying to register Manager which is already registered!" );
            return ( nullptr );
        }

        Manager* mgrPtr{ new MgrT( this, std::forward<ArgsT>( args )... ) };
        mManagers.insert< typename MgrT::AssetTypeT >( mgrPtr );

        return ( static_cast< MgrT* >( mgrPtr ) );
    }

    template < typename MgrT >
    MgrT* getMgr() const noexcept
    {
        static_assert(
            std::is_base_of< Mgr< typename MgrT::AssetTypeT >, MgrT >::value
            , "MgrT should extend tktk::asset::Mgr<>"
        );

        auto it( mManagers.find< MgrT::AssetTypeT >() );
        if ( it != mManagers.end() )
        {
            Manager* mgrPtr{ it->second };
            return ( static_cast< MgrT* >( mgrPtr ) );
        }

        return ( nullptr );
    }

    template< typename AssetT >
    Mgr< AssetT >* getMgrForAssetType() noexcept
    {
        static_assert(
            std::is_base_of< Manager , Mgr< AssetT > >::value
            , "Mgr< AssetT > type derived from Manager doesnt exist for given AssetT"
        );
        auto it( mManagers.find< AssetT >() );
        if ( it == mManagers.end() )
        {
            return ( nullptr );
        }
        Manager* mgrPtr{ it->second };
        return ( static_cast< Mgr< AssetT >* >( mgrPtr ) );
    }


    template < typename AssetT >
    std::shared_ptr< AssetT > load( const std::string& name, bool reload = false ) noexcept
    {
        std::shared_ptr< AssetT > shared;

        Mgr< AssetT >* mgrPtr{ getMgrForAssetType< AssetT >() };
        if ( mgrPtr == nullptr )
        {
            assert( false && "Manager for AssetT asset type isnt registered within the system instance." );
        }
        else
        {
            shared = mgrPtr->load( name, reload );
        }

        return ( shared );
    }

    template< typename AssetT >
    std::shared_ptr< AssetT > get( const std::string& name ) noexcept
    {
        std::shared_ptr< AssetT > shared;

        Mgr< AssetT >* mgrPtr{ getMgrForAssetType< AssetT >() };
        if ( mgrPtr == nullptr )
        {
            assert( false && "Manager for AssetT asset type isnt registered within the system instance." );
        }
        else
        {
            shared = mgrPtr->get( name );
        }

        return ( shared );
    }

    template< typename AssetT >
    std::shared_ptr< AssetT > drop( const std::string& name ) noexcept
    {
        std::shared_ptr< AssetT > shared;

        Mgr< AssetT >* mgrPtr{ getMgrForAssetType< AssetT >() };
        if ( mgrPtr == nullptr )
        {
            assert( false && "Manager for AssetT asset type isnt registered within the system instance." );
        }
        else
        {
            shared = mgrPtr->drop( name );
        }

        return ( shared );
    }

protected:
    util::TypeMap< Manager* > mManagers;
};

} //namespace asset
} //namespace tktk

#endif /* end of include guard: TKTK_ASSET_SYSTEM_HPP */
