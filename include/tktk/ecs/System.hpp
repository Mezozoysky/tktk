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

#ifndef TKTK_ECS_SYSTEM_HPP
#define TKTK_ECS_SYSTEM_HPP

#include <tktk/Config.hpp>
#include <type_traits>
#include <memory>
#include <algorithm>
#include <tktk/ecs/Processor.hpp>
#include <tktk/util/TypeMap.hpp>
#include <tktk/util/Signal.hpp>

namespace tktk
{
namespace ecs
{


class System
{
public:
    using PoolTypeT = util::MemoryPool< Entity >;

    util::Signal< float > updateSignal;

    System() noexcept
    {
    }

    virtual ~System() noexcept
    {
        // Unregister and destroy processors
        auto it( mProcessors.begin() );
        while ( it != mProcessors.end() )
        {
            auto procPtr( it->second );
            delete procPtr;
            mProcessors.remove( it++ );
        }
    }

    virtual void setup() noexcept
    {
        auto it = mProcessors.begin();
        while ( it != mProcessors.end() )
        {
            it->second->setup( this );
            ++it;
        }
    }

    virtual void update( float deltaTime ) noexcept
    {
        updateSignal( deltaTime );
    }

    ///
    /// About entity
    ///

    Entity::Handle addEntity() noexcept
    {
        util::Id64 eId{ mEntityPool.createElement() };
        Entity::Handle handle( eId, this );
        return ( handle );
    }

    void removeEntity( Entity::Handle& handle ) noexcept
    {
        mEntityPool.destroyElement( handle.getId() );
        handle.invalidate();
    }

    // for use from entity handle
    void removeEntity( const util::Id64& eId  ) noexcept
    {
        mEntityPool.destroyElement( eId );
    }

    bool isEntityValid( const Entity::Handle& handle ) const noexcept
    {
        return ( mEntityPool.isIdValid( handle.getId() ) );
    }

    // for use from entity handle
    bool isIdValid( const util::Id64& eId ) const noexcept
    {
        return ( mEntityPool.isIdValid( eId ) );
    }

    Entity* getEntityPtr( const Entity::Handle& handle ) const noexcept
    {
        return ( getEntityPtr( handle.getId() ) );
    }

    // for use from entity handle
    Entity* getEntityPtr( const util::Id64& eId ) const noexcept
    {
        if ( !mEntityPool.isIdValid( eId ) )
        {
            return ( nullptr );
        }
        return ( mEntityPool.getPtr( eId.index() ) );
    }

    ///
    /// About components
    ///

    ///
    /// About processors
    ///


    template< typename T, typename... TArgs >
    T* registerProcessor( TArgs&&... args )
    {
        static_assert(
                      std::is_base_of< ProcessorBase, T >::value
                      , "T should extend tktk::ecs::ProcessorBase"
        );

        auto procPtr = new T( std::forward< TArgs >( args )... );
        mProcessors.insert< typename T::CompTypeT >( static_cast< ProcessorBase* >( procPtr ) );

        return ( procPtr );
    }

    template< typename T >
    T* getProcessor()
    {
        static_assert(
                      std::is_base_of< ProcessorBase, T >::value
                      , "T should extend tktk::ecs::ProcessorBase"
        );

        T* procPtr;

        auto it = mProcessors.find< typename T::CompTypeT >();
        if ( it != mProcessors.end() )
        {
            procPtr = static_cast< T* >( it->second );
        }

        return ( procPtr );
    }

    template< typename T >
    Processor< T >* getProcessorForCompType()
    {
        static_assert(
            std::is_base_of< ComponentBase, T >::value
            , "T should extend tktk::ecs::ComponentBase"
        );

        Processor< T >* procPtr;

        auto it = mProcessors.find< T >();
        if ( it != mProcessors.end() )
        {
            procPtr = static_cast< Processor< T >* >( it->second );
        }

        return ( procPtr );
    }

protected:
    PoolTypeT mEntityPool;

    util::TypeMap< ProcessorBase* > mProcessors;
};

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_SYSTEM_HPP */
