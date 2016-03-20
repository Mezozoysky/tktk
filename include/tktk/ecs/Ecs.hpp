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
/// \brief This file provides Ecs type
/// \author Stanislav Demyanovich <stan@angrybubo.com>
/// \date 2015-2016
/// \copyright tktk is released under the terms of zlib/png license; see full license text at https://opensource.org/licenses/Zlib


#ifndef TKTK_ECS_ECS_HPP
#define TKTK_ECS_ECS_HPP

#include <tktk/ecs/Config.hpp>
#include <tktk/ecs/SystemB.hpp>
#include <tktk/ecs/CompMgr.hpp>
#include <tktk/ecs/EntityMgr.hpp>

namespace tktk
{
namespace ecs
{

// Forwad declarations
class Ecs;

struct EntityHandle
{
    EntityHandle() noexcept;

    explicit EntityHandle( mpool::Id64 id, Ecs* ecs ) noexcept;

    void drop() noexcept;

    inline bool isValid() const noexcept;

    inline void invalidate() noexcept;

private:

    mpool::Id64 mId{ mpool::ID64_INVALID };
    Ecs* mEcs{ nullptr };
};

template< typename CompT >
struct CompHandle
{

    CompHandle() noexcept
    {
    }

    explicit CompHandle( mpool::Id64 eId, Ecs* ecs ) noexcept
    : mEId{ eId }
    , mEcs{ ecs }
    {
    }

    CompT* operator->() noexcept;


    inline bool isValid() const noexcept;

    inline void invalidate() noexcept;

private:

    mpool::Id64 mEId{ mpool::ID64_INVALID };
    Ecs* mEcs{ nullptr };
};

class Ecs
: protected SystemB< TKTK_ECS_CONFIG_COMPS_PER_ENTITY >
{
    using SystemT = SystemB< TKTK_ECS_CONFIG_COMPS_PER_ENTITY >;

public:

    static const std::size_t COMPS_PER_ENTITY{ TKTK_ECS_CONFIG_COMPS_PER_ENTITY };

    /// \brief Default constructor
    Ecs() noexcept
    : SystemB()
    {
        mCompArraysByEntityIndex.reserve( mEntityMgr.getCapacity() );
    }

    /// \brief Virtual destructor
    virtual ~Ecs() noexcept
    {
    }

    template< typename CompT, typename MgrT, typename... MgrArgsT >
    bool regCompType( MgrArgsT&&... args ) noexcept
    {
        static_assert(
            std::is_base_of< CompMgr< CompT >, MgrT >::value
            , "MgrT should extend tktk::ecs::CompMgr<CompT> template"
        );

        std::size_t index{ SystemT::regDataType( std::forward< MgrArgsT >( args )... ) };
        if ( index >= DT_INDEX_INVALID )
        {
            return ( false );
        }
        return ( true );
    }



    /// \brief Adds 'new' entity
    /// \returns The handle for added entity.
    ///
    /// If there is no 'dead' entities in the memory the new entity will be created, othervise first of the 'dead' entities will be reused.
    EntityHandle add() noexcept
    {
        mpool::Id64 id{ mEntityMgr.add() };
        assert( mEntityMgr.isIdValid( id ) && "Newly allocated entity id is invalid" );

        // Provide the entity->components mapping consistency
        if ( mCompArraysByEntityIndex.capacity() < mEntityMgr.getCapacity() )
        {
            mCompArraysByEntityIndex.reserve( mEntityMgr.getCapacity() );
        }
        if ( mCompArraysByEntityIndex.size() == id.index() )
        {
            mCompArraysByEntityIndex.resize( id.index() + 1 );
        }
        else if ( mCompArraysByEntityIndex.size() < id.index() ) // just for debug
        {
            assert( false && "New entity index isnt accord with entity-component mapping (<)" );
        }
        mCompArraysByEntityIndex[ id.index() ].fill( mpool::ID64_INVALID );

        EntityHandle handle( id, this );
        return ( handle );
    }

    /// \brief Remove the entity by entity's id
    /// \param[in] eId Specifies the entity to remove
    ///
    /// Adds the entity specified with id to the 'dead-list'.
    /// \note Unless you are modifying the tktk-ecs library you should not use this method, use `Entity::Handle::remove()` instead.
    inline void drop( mpool::Id64 id  ) noexcept
    {
        mEntityMgr.drop( id );
    }

    /// \brief Tests if entity id is valid
    /// \param[in] eId The id to test
    /// \returns `true` if id is valid, `false` othervise
    /// \note Unless you are modifying the tktk-ecs library you should not use this method, use `Entity::Handle::isValid()` instead.
    inline bool isValid( mpool::Id64 id ) const noexcept
    {
        return ( mEntityMgr.isIdValid( id ) );
    }

    /// \brief Gets the raw pointer to the entity by entity's id
    /// \param[in] eId entity's id
    /// \returns The pointer to the entity's memory
    /// \note Unless you are modifying the tktk-ecs library you should not use this method, use `Entity::Handle::operator->()` instead.
    inline Entity* getPtr( mpool::Id64 id ) const noexcept
    {
        return ( mEntityMgr.getDataPtr( id ) );
    }

    template< typename CompT >
    inline CompT* getCompPtr( mpool::Id64 id ) const noexcept
    {
        assert( isValid( id ) && "Trying to get comp ptr from invalid entity" );

        std::size_t cIndex{ getDataTypeIndex< CompT >() };
        mpool::Id64 cId{ mCompArraysByEntityIndex[ id.index() ][ cIndex ] };

        CompMgr< CompT >* mgrPtr{ static_cast< CompMgr< CompT >* >( SystemT::getMgrPtr( cIndex ) ) };
        assert( mgrPtr != nullptr && "Trying to get comp ptr of comp type which is not registered." );

        if ( !mgrPtr->isIdValid( cId ) )
        {
            return ( nullptr );
        }
        return ( mgrPtr->getDataPtr( cId ) );
    }

    template< typename CompT >
    inline bool isCompValid( mpool::Id64 id ) const noexcept
    {
        assert( isValid( id ) && "Trying to check comp validity from invalid entity" );
        std::size_t cIndex{ getDataTypeIndex< CompT >() };
        mpool::Id64 cId{ mCompArraysByEntityIndex[ id.index() ][ cIndex ] };

        CompMgr< CompT >* mgrPtr{ static_cast< CompMgr< CompT >* >( getMgrPtr( cIndex ) ) };
        assert( mgrPtr != nullptr && "Trying to check validity of comp which type is not registered." );

        return ( mgrPtr->isIdValid( cId ) );
    }

private:

    using CompArrayT = std::array< mpool::Id64, TKTK_ECS_CONFIG_COMPS_PER_ENTITY >;
    EntityMgr mEntityMgr;
    std::vector< CompArrayT > mCompArraysByEntityIndex;
};

//
// Template-methods realisations
//

template< typename CompT >
CompT* CompHandle< CompT >::operator->() noexcept
{
    if ( !isValid() )
    {
        return ( nullptr );
    }
    return ( mEcs->getCompPtr< CompT >( mEId ) );
}

template< typename CompT >
inline bool CompHandle< CompT >::isValid() const noexcept
{
    return ( mEcs != nullptr && mEcs->isValid( mEId ) && mEcs->isCompValid< CompT >( mEId ) );
}

template< typename CompT >
inline void CompHandle< CompT >::invalidate() noexcept
{
    mEId = mpool::ID64_INVALID;
    mEcs = nullptr;
}


} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_ECS_HPP */
