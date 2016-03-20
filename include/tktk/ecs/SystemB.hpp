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
/// \brief This file provides ecs::SystemB type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2015-2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib


#ifndef TKTK_ECS_SYSTEM_B_HPP
#define TKTK_ECS_SYSTEM_B_HPP

#include <tktk/ecs/Config.hpp>
#include <tktk/ecs/Mgr.hpp>
#include <type_traits>
#include <cassert>
#include <memory>
#include <cstdint>
#include <tktk/util/TypeMap.hpp>

namespace tktk
{
namespace ecs
{


template < std::size_t maxDataTypesV = 64 >
class SystemB
{
public:

    static const std::size_t DT_INDEX_INVALID{ maxDataTypesV };

    /// \brief Default constructor
    ///
    /// Fills mgrs array with nullptr
    SystemB() noexcept
    {
        mMgrs.fill( nullptr );
    }

    /// \brief Virtual destructor for further inheritance
    ///
    /// Destroys all the registered Managers.
    virtual ~SystemB() noexcept
    {
        for ( std::size_t index = 0; index < mMgrs.size(); ++ index )
        {
            if ( mMgrs[ index ] != nullptr )
            {
                delete mMgrs[ index ];
                mMgrs[ index ] = nullptr;
            }
        }
    }

    template< typename DataT, typename MgrT, typename... MgrArgsT >
    std::size_t regDataType( MgrArgsT&&... args ) noexcept
    {
        static_assert(
            std::is_base_of< Mgr< DataT >, MgrT >::value
            , "MgrT should extend tktk::ecs::Manager<DataT> template"
        );

        if ( mMgrsByType.count< DataT >() == 1 )
        {
            ll_error( "Trying to register Manager which is already registered!" );
            return ( DT_INDEX_INVALID );
        }

        std::size_t index{ mNextTypeIndex++ };
        assert( index < maxDataTypesV && "Manager index >= maxDataTypesV !!!" );
        if ( index >= maxDataTypesV )
        {
            ll_error( "Cant register data type because of maxDataTypesV limit" );
            return ( DT_INDEX_INVALID );
        }

        MgrT* mgrPtr{ new MgrT( this, std::forward< MgrArgsT >( args )... ) };
        assert( mgrPtr != nullptr && "Cant register data type because of constructor failure" );
        if ( mgrPtr == nullptr )
        {
            ll_error( "Cant register data type because of constructor failure" );
            return ( DT_INDEX_INVALID );
        }
        mMgrs[ index ] = mgrPtr;
        mMgrsByType.insert< DataT >( index );

        return ( index );
    }

    template< typename MgrT >
    MgrT* getMgrPtr() noexcept
    {
        static_assert(
            std::is_base_of< Mgr< typename MgrT::DataTypeT >, MgrT >::value
            , "T should extend tktk::ecs::Mgr<DataT> template"
        );

        static std::size_t cachedMgrIndex{ DT_INDEX_INVALID };
        MgrT* mgrPtr{ nullptr };
        if ( cachedMgrIndex >= DT_INDEX_INVALID /*|| shouldRecacheIndices */ )
        {
            auto it( mMgrsByType.find< typename MgrT::DataTypeT >() );
            if ( it == mMgrsByType.end() )
            {
                return ( mgrPtr ); //nullptr
            }
            cachedMgrIndex = it->second;
        }
        return ( static_cast< MgrT* >( mMgrs[ cachedMgrIndex ] ) );
    }

    Manager* getMgrPtr( std::size_t index ) const noexcept
    {
        if ( index >= DT_INDEX_INVALID )
        {
            return ( nullptr );
        }
        return ( mMgrs[ index ] );
    }

    template< typename DataT >
    inline std::size_t getDataTypeIndex() noexcept
    {
        static std::size_t cachedMgrIndex{ DT_INDEX_INVALID };
        if ( cachedMgrIndex >= DT_INDEX_INVALID /*|| shouldRecacheIndices */ )
        {
            auto it( mMgrsByType.find< DataT >() );
            if ( it != mMgrsByType.end() )
            {
                cachedMgrIndex = it->second;
            }
        }
        return ( cachedMgrIndex );
    }

    template< typename DataT >
    Mgr< DataT >* getMgrPtrForDataType() noexcept
    {
        static_assert(
            std::is_base_of< Manager, Mgr< DataT > >::value
            , "Mgr< DataT > type derived from Manager doesnt exist for given DataT"
        );

        static std::size_t dtIndex{ getDataTypeIndex< DataT >() };
        Mgr<DataT>* mgrPtr{ nullptr };
        if ( dtIndex < DT_INDEX_INVALID )
        {
            mgrPtr = static_cast< Mgr< DataT >* >( mMgrs[ dtIndex ] );
        }
        return ( mgrPtr );
    }

private:
    std::array< Manager*, maxDataTypesV > mMgrs; ///< Registered managers in registering order
    std::size_t mNextTypeIndex{ 0 };
    util::TypeMap< std::size_t > mMgrsByType; ///< Managers' indexes by accoding component types map
};

//
// Template-methods realisations
//

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_SYSTEM_B_HPP */
