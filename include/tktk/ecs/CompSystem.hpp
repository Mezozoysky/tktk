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
#include <type_traits>
#include <cassert>
#include <memory>
#include <array>
#include <cstdint>
#include <tktk/util/TypeMap.hpp>
#include <tktk/mpool/MemoryPool.hpp>

namespace tktk
{
namespace ecs
{

/// \brief Marker "interface" for storing `CompSystem`s with std containers
class CompSystemMarker
{
    CompSystemMarker( const CompSystemMarker& ) = delete;
    CompSystemMarker& operator= ( const CompSystemMarker& ) = delete;

public:
    /// \brief Default constructor
    CompSystemMarker() noexcept = default;
    /// \brief Virtual destructor for further inheritance
    virtual ~CompSystemMarker() noexcept = default;

    virtual bool setup() noexcept = 0;
    virtual void shutdown() noexcept = 0;
};

/// \brief Marker "interface" for storing `CompMgr`s with std containers
class CompMgrMarker
{
    CompMgrMarker( const CompMgrMarker& ) = delete;
    CompMgrMarker& operator= ( const CompMgrMarker& ) = delete;

public:
    /// \brief Default constructor
    CompMgrMarker() noexcept = default;
    /// \brief Virtual destructor for further inheritance
    virtual ~CompMgrMarker() noexcept = default;

    virtual bool setup() noexcept = 0;
    virtual void shutdown() noexcept = 0;
};

// Forward declaration
template< std::size_t maxCompTypesV>
class CompSystem;

template < typename CompT >
class CompMgr
: public CompMgrMarker
{
public:
    /// \brief Wraps the managing data type
    using CompTypeT = CompT;

    /// \brief Default constructor
    CompMgr() noexcept
    : CompMgrMarker()
    {
        mPool.reserve( 1 );
    }

    /// \brief Virtual destructor for further inheritance
    ///
    /// Does nothing
    virtual ~CompMgr() noexcept
    {
    }

    /// \brief Adds component with constructructor arguments \em args and returns a handle of type CompTypeT::Handle
    template< typename... ArgsT >
    inline mpool::Id64 add( ArgsT&&... args )
    {
        return ( mPool.alloc( std::forward< ArgsT >( args )... ) );
    }

    /// \brief Marks \em CompTypeT object specified by \em id as free if \em id is valid
    inline void drop( mpool::Id64 id )
    {
        mPool.free( id );
    }

    /// \brief Returns true if given id is valid
    inline bool isIdValid( mpool::Id64 id ) const noexcept
    {
        return ( mPool.isIdValid( id ) );
    }

    /// \brief Returns raw pointer to the data by given \em id
    inline CompT* getDataPtr( mpool::Id64 id ) const noexcept
    {
        return ( mPool.getPtr( id ) );
    }

    inline std::size_t getSize() const noexcept
    {
        return ( mPool.getSize() );
    }

    inline std::size_t getCapacity() const noexcept
    {
        return ( mPool.getCapacity() );
    }

    inline void forEach( std::function< void( CompT& comp ) > func )
    {
        mPool.forEach( func );
    }

protected:

    using PoolTypeT = mpool::MemoryPool<CompT>;

private:

    PoolTypeT mPool;
};


template< std::size_t maxCompTypesV = 64 >
class CompSystem
: public CompSystemMarker
{
public:

    static constexpr std::size_t MAX_DATA_TYPES{ maxCompTypesV };

    /// \brief Default constructor
    ///
    /// Fills mgrs array with nullptr
    CompSystem() noexcept
    : CompSystemMarker()
    {
        mMgrs.fill( nullptr );
    }

    /// \brief Virtual destructor for further inheritance
    ///
    /// Destroys all the registered CompMgrMarkers.
    virtual ~CompSystem() noexcept
    {
        for ( std::size_t index = 0; index < mMgrs.size(); ++index )
        {
            if ( mMgrs[ index ] != nullptr )
            {
                delete mMgrs[ index ];
                mMgrs[ index ] = nullptr;
            }
        }
    }

    virtual bool setup() noexcept override
    {
        bool res{ true };
        for ( std::size_t index = 0; index < mMgrs.size(); ++index )
        {
            if ( mMgrs[ index ] != nullptr )
            {
                res = mMgrs[ index ]->setup();
                if ( !res )
                {
                    break;
                }
            }
        }

        return ( res );
    }

    virtual void shutdown() noexcept override
    {
        for ( std::size_t index = 0; index < mMgrs.size(); ++index )
        {
            if ( mMgrs[ index ] != nullptr )
            {
                mMgrs[ index ]->shutdown();
            }
        }
    }

    template< typename CompT, typename MgrT, typename... MgrArgsT >
    std::size_t regCompType( MgrArgsT&&... args ) noexcept
    {
        static_assert(
            std::is_base_of< CompMgr< CompT >, MgrT >::value
            , "MgrT should extend CompMgr<CompT> template"
        );

        if ( mMgrsByType.count< CompT >() == 1 )
        {
            ll_error( "Trying to register Manager which is already registered!" );
            return ( MAX_DATA_TYPES );
        }

        std::size_t index{ mNextTypeIndex++ };
        assert( index < maxCompTypesV && "Manager index >= maxCompTypesV !!!" );
        if ( index >= maxCompTypesV )
        {
            ll_error( "Cant register data type because of maxCompTypesV limit" );
            return ( MAX_DATA_TYPES );
        }

        MgrT* mgrPtr{ new MgrT( std::forward< MgrArgsT >( args )... ) };
        assert( mgrPtr != nullptr && "Cant register data type because of constructor failure" );
        if ( mgrPtr == nullptr )
        {
            ll_error( "Cant register data type because of constructor failure" );
            return ( MAX_DATA_TYPES );
        }
        mMgrs[ index ] = mgrPtr;
        mMgrsByType.insert< CompT >( index );

        return ( index );
    }

    template< typename MgrT >
    MgrT* getMgrPtr() noexcept
    {
        static_assert(
            std::is_base_of< CompMgr< typename MgrT::CompTypeT >, MgrT >::value
            , "T should extend CompMgr<CompT> template"
        );

        static std::size_t cachedMgrIndex{ MAX_DATA_TYPES };
        MgrT* mgrPtr{ nullptr };
        if ( cachedMgrIndex >= MAX_DATA_TYPES /*|| shouldRecacheIndices */ )
        {
            auto it( mMgrsByType.find< typename MgrT::CompTypeT >() );
            if ( it == mMgrsByType.end() )
            {
                return ( mgrPtr ); //nullptr
            }
            cachedMgrIndex = it->second;
        }
        return ( static_cast< MgrT* >( mMgrs[ cachedMgrIndex ] ) );
    }

    CompMgrMarker* getMgrPtr( std::size_t index ) const noexcept
    {
        if ( index >= MAX_DATA_TYPES )
        {
            return ( nullptr );
        }
        return ( mMgrs[ index ] );
    }

    template< typename CompT >
    inline std::size_t getCompTypeIndex() const noexcept
    {
        static std::size_t cachedMgrIndex{ MAX_DATA_TYPES };
        if ( cachedMgrIndex >= MAX_DATA_TYPES /*|| shouldRecacheIndices */ )
        {
            auto it( mMgrsByType.find< CompT >() );
            if ( it != mMgrsByType.end() )
            {
                cachedMgrIndex = it->second;
            }
        }
        return ( cachedMgrIndex );
    }

    template< typename CompT >
    CompMgr< CompT >* getMgrPtrForCompType() noexcept
    {
        static_assert(
            std::is_base_of< CompMgrMarker, CompMgr< CompT > >::value
            , "Mgr< CompT > type derived from Manager doesnt exist for given CompT"
        );

        static std::size_t dtIndex{ getCompTypeIndex< CompT >() };
        CompMgr<CompT>* mgrPtr{ nullptr };
        if ( dtIndex < MAX_DATA_TYPES )
        {
            mgrPtr = static_cast< CompMgr< CompT >* >( mMgrs[ dtIndex ] );
        }
        return ( mgrPtr );
    }

private:
    std::array< CompMgrMarker*, maxCompTypesV > mMgrs; ///< Registered managers in registering order
    std::size_t mNextTypeIndex{ 0 };
    util::TypeMap< std::size_t > mMgrsByType; ///< Managers' indexes by accoding component types map
};

//
// Template-methods realisations
//

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_SYSTEM_B_HPP */
