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
#include <tktk/ecs/Entity.hpp>
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

    System() noexcept;
    virtual ~System() noexcept;

    virtual void setup() noexcept;

    virtual void update( float deltaTime ) noexcept
    {
        updateSignal( deltaTime );
    }

    ///
    /// About entity
    ///

    Entity::Handle addEntity() noexcept;

    void removeEntity( Entity::Handle& eHandle ) noexcept;
    // for use from entity handle
    void removeEntity( const util::Id64& eId  ) noexcept;

    bool isEntityValid( const Entity::Handle& handle ) const noexcept;
    // for use from entity handle
    bool isIdValid( const util::Id64& eId ) const noexcept;

    Entity* getEntityPtr( const Entity::Handle& handle ) const noexcept;
    // for use from entity handle
    Entity* getEntityPtr( const util::Id64& eId ) const noexcept;

    ///
    /// About components
    ///

    template< typename T, typename... TArgs >
    typename T::Handle addComponent( ecs::Entity::Handle& eHandle, TArgs&&... args )
    {
        auto procPtr( getProcessorForCompType< T >() );
        assert( procPtr && "Processor for given component type is not registered." );

        typename T::Handle cHandle{ procPtr->addComponent( eHandle.getId(), std::forward< TArgs >( args )... ) };

        if ( eHandle->map.find< T >() != eHandle->map.end() )
        {
            assert( false && "Type already exists!" );
        }
        eHandle->map.insert< T >( cHandle.getUntyped() );

        return ( cHandle );
    }

    template< typename T >
    void removeComponent( const ecs::Entity::Handle& eHandle )
    {
        ll_trace( "method in;" );

        if ( !eHandle.isValid() )
        {
            ll_trace( "entity handle is invalid; method out;" );
            return;
        }
        ll_trace( "entity handle is valid" );
        auto procPtr( getProcessorForCompType< T >() );
        assert( procPtr && "Processor for given component type is not registered." );
        ll_trace( "processor retreived" );
        auto it = eHandle->map.find< T >();
        if ( it == eHandle->map.end() )
        {
            ll_trace( "entity has not cId for type T" );
        }
        else
        {
            ll_trace( "entity has cId for T" );
            Component::Handle ucHandle{ eHandle->map.find< T >()->second };
            ll_trace(  "comp id retreived: " << ucHandle.getId().index() );
            procPtr->destroyElement( ucHandle.getId() );
            ll_trace( "component by cId has destroyed" );
            eHandle->map.remove< T >();
            ll_trace( "T removed from entity map" );
        }
        ll_trace(  "all done; method out" );
    }

    template< typename T >
    typename T::Handle getComponent( const ecs::Entity::Handle& eHandle )
    {
        typename T::Handle invalidCHandle{};

        if ( !eHandle.isValid() )
        {
            return ( invalidCHandle );
        }

        auto it = eHandle->map.find< T >();
        if ( it == eHandle->map.end() )
        {
            return ( invalidCHandle );
        }
        Component::Handle ucHandle{ it->second };

        Proc< T >* procPtr{ getProcessorForCompType< T >() };
        assert( procPtr && "Processor for given component type is not registered." );

        if ( !procPtr->isIdValid( ucHandle.getId() ) )
        {
            return ( invalidCHandle );
        }

        typename T::Handle cHandle{ ucHandle };
        return ( cHandle );
    }


    ///
    /// About processors
    ///

    template< typename T, typename... TArgs >
    T* registerProcessor( TArgs&&... args )
    {
        static_assert(
                      std::is_base_of< Processor, T >::value
                      , "T should extend tktk::ecs::Processor"
        );

        Processor* procPtr = new T( std::forward< TArgs >( args )... );
        mProcessors.insert< typename T::CompTypeT >( procPtr );

        return ( static_cast< T* >( procPtr ) );
    }

    template< typename T >
    T* getProcessor()
    {
        static_assert(
                      std::is_base_of< Processor, T >::value
                      , "T should extend tktk::ecs::Processor"
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
    Proc< T >* getProcessorForCompType()
    {
        static_assert(
            std::is_base_of< Component, T >::value
            , "T should extend tktk::ecs::Component"
        );

        Proc< T >* procPtr;

        auto it = mProcessors.find< T >();
        if ( it != mProcessors.end() )
        {
            procPtr = static_cast< Proc< T >* >( it->second );
        }

        return ( procPtr );
    }

protected:
    PoolTypeT mEntityPool;

    util::TypeMap< Processor* > mProcessors;
};

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_SYSTEM_HPP */
