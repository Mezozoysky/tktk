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

#include <type_traits>
#include <memory>
#include <algorithm>
#include <tktk/ecs/Processor.hpp>
#include <tktk/ecs/EntityManager.hpp>
#include <tktk/ecs/EventProxy.hpp>
#include <tktk/util/TypeMap.hpp>

namespace tktk
{
namespace ecs
{

template< typename EventProxyT = EventProxy, typename EntityManagerT = EntityManager >
class System
{
public:

    System() noexcept
    {
        static_assert(
                      std::is_base_of< EventProxy, EventProxyT >::value
                      , "EventProxyT should extend tktk::ecs::EventProxy"
        );
        static_assert(
                      std::is_base_of< EntityManager, EntityManagerT >::value
                      , "EntityManagerT should extend tktk::ecs::EntityManager"
        );
    }

    virtual ~System() = default;

    virtual void setup() noexcept
    {
        auto it = mProcessors.begin();
        while ( it != mProcessors.end() )
        {
            it->second->setup( mEventProxy, mEntityManager );
            ++it;
        }
    }

    virtual void update( float deltaTime ) noexcept
    {
        mEventProxy.updateSignal( deltaTime );
    }

    template< typename T, typename... TArgs >
    std::shared_ptr< T > registerProcessor( TArgs&&... args )
    {
        static_assert(
                      std::is_base_of< ProcessorBase, T >::value
                      , "T should extend tktk::ecs::ProcessorBase"
        );

        auto processor = std::make_shared< T >( std::forward< TArgs >( args )... );
        mProcessors.insert< T >( processor );

        return ( processor );
    }

    template< typename T >
    std::shared_ptr< T > getProcessor()
    {
        static_assert(
                      std::is_base_of< ProcessorBase, T >::value
                      , "T should extend tktk::ecs::ProcessorBase"
        );

        std::shared_ptr< T > processor;

        auto it = mProcessors.find< T >();
        if ( it != mProcessors.end() )
        {
            processor = std::static_pointer_cast< T >( it->second );
        }

        return ( processor );
    }

    EntityManagerT& getEntityManager()
    {
        return ( mEntityManager );
    }

    const EntityManagerT& getEntityManager() const
    {
        return ( mEntityManager );
    }

    EventProxyT& getEventProxy()
    {
        return ( mEventProxy );
    }

    const EventProxyT& getEventProxy() const
    {
        return ( mEventProxy );
    }

private:
    EntityManagerT mEntityManager;
    EventProxyT mEventProxy;

    util::TypeMap< std::shared_ptr< ProcessorBase > > mProcessors;
};

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_SYSTEM_HPP */
