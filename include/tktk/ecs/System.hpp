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

    template< typename T, typename... ArgsT >
    typename T::Handle addComp( const Entity::Handle& eHandle, ArgsT&&... args )
    {
        return ( addComp< T >( eHandle.getId() ) );
    }

    // for use from handle
    /// Adds component of given type @T with constructor arguments @args to the entity specified by id @eId
    template< typename T, typename... ArgsT >
    typename T::Handle addComp( const util::Id64& eId, ArgsT&&... args )
    {
        auto procPtr( getProcForCompType< T >() );
        assert( procPtr && "Processor for given component type is not registered." );

        typename T::Handle cHandle{ procPtr->addComponent( eId, std::forward< ArgsT >( args )... ) };

        Entity* ePtr{ getEntityPtr( eId ) };
        if ( ePtr->map.find< T >() != ePtr->map.end() )
        {
            assert( false && "Type already exists!" );
        }
        ePtr->map.insert< T >( cHandle.getUntyped() );

        return ( cHandle );
    }

    template< typename T >
    typename T::Handle getComp( const Entity::Handle& eHandle )
    {
        return ( getComp< T >( eHandle.getId() ) );
    }

    // for use from handles
    /// Returns handle for component of givent type @T from entity specified by id @eId; If entity has not component of the given type returned handle will be invalid
    template< typename T >
    typename T::Handle getComp( const util::Id64& eId )
    {
        typename T::Handle invalidCHandle{};

        if ( !isIdValid( eId ) )
        {
            return ( invalidCHandle );
        }

        Entity* ePtr{ getEntityPtr( eId ) };
        auto it = ePtr->map.find< T >();
        if ( it == ePtr->map.end() )
        {
            return ( invalidCHandle );
        }
        Component::Handle ucHandle{ it->second };

        Proc< T >* procPtr{ getProcForCompType< T >() };
        assert( procPtr && "Processor for given component type is not registered." );

        if ( !procPtr->isIdValid( ucHandle.getId() ) )
        {
            return ( invalidCHandle );
        }

        typename T::Handle cHandle{ ucHandle };
        return ( cHandle );
    }

    template< typename T >
    void removeComp( const Entity::Handle& eHandle )
    {
        removeComp< T >( eHandle.getId() );
    }

    // for use from handles
    /// Removes component of given type @T from the entity specified by id @eId if added.
    template< typename T >
    void removeComp( const util::Id64& eId )
    {
        if ( !isIdValid( eId ) )
        {
            return;
        }
        auto procPtr( getProcForCompType< T >() );
        assert( procPtr && "Processor for given component type is not registered." );

        Entity* ePtr{ getEntityPtr( eId ) };
        auto it = ePtr->map.find< T >();
        if ( it == ePtr->map.end() )
        {
            return;
        }

        Component::Handle ucHandle{ ePtr->map.find< T >()->second };
        procPtr->destroyElement( ucHandle.getId() );
        ePtr->map.remove( it );
    }

    ///
    /// About processors
    ///

    template< typename T, typename... ArgsT >
    T* registerProc( ArgsT&&... args )
    {
        static_assert(
                      std::is_base_of< Processor, T >::value
                      , "T should extend tktk::ecs::Processor"
        );

        Processor* procPtr{ new T( std::forward< ArgsT >( args )... ) };
        mProcessors.insert< typename T::CompTypeT >( procPtr );

        return ( static_cast< T* >( procPtr ) );
    }

    template< typename T >
    T* getProc()
    {
        static_assert(
                      std::is_base_of< Processor, T >::value
                      , "T should extend tktk::ecs::Processor"
        );

        T* procPtr{ nullptr };

        auto it = mProcessors.find< typename T::CompTypeT >();
        if ( it != mProcessors.end() )
        {
            procPtr = static_cast< T* >( it->second );
        }

        return ( procPtr );
    }

    template< typename CompT >
    Proc< CompT >* getProcForCompType()
    {
        static_assert(
            std::is_base_of< Component, CompT >::value
            , "T should extend tktk::ecs::Component"
        );

        Proc< CompT >* procPtr;

        auto it = mProcessors.find< CompT >();
        if ( it != mProcessors.end() )
        {
            procPtr = static_cast< Proc< CompT >* >( it->second );
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
