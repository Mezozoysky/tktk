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

#ifndef TKTK_ECS_PROCESSOR_HPP
#define TKTK_ECS_PROCESSOR_HPP

#include <tktk/util/MemoryPool.hpp>
#include <tktk/ecs/Component.hpp>
#include <tktk/ecs/EventProxy.hpp>
#include <tktk/ecs/EntityManager.hpp>
#include <memory>

namespace tktk
{
namespace ecs
{

struct Entity;


class ProcessorBase
: public std::enable_shared_from_this< ProcessorBase >
{
    ProcessorBase( const ProcessorBase& ) = delete;
    ProcessorBase& operator= ( const ProcessorBase& ) = delete;

public:

    ProcessorBase() = default;
    virtual ~ProcessorBase() = default;

    virtual void setup( EventProxy& eventProxy, EntityManager& entityManager ) = 0;
    virtual void onUpdate( float deltaTime ) = 0;
//    virtual void onFixedUpdate( float deltaTime ) = 0;

//    virtual void isComponentValid( std::size_t index ) const noexcept = 0;
//    virtual void
};


template< typename ProcT, typename CompT >
class Processor
: public ProcessorBase
{

public:

    using CompType = CompT;
    using Type = ProcT;
    using HandleType = ComponentHandle< Type >;

    Processor()
    : ProcessorBase()
    {
    }

    virtual ~Processor() noexcept
    {
    }

    virtual void setup( EventProxy& eventProxy, EntityManager& entityManager ) override
    {
        eventProxy.updateSignal.connect( std::bind( &ProcessorBase::onUpdate, this, std::placeholders::_1 ) );
    }

    template< typename... Args >
    ComponentHandle< Type > addComponent( Args&&... args )
    {
        std::size_t index{ mComponents.createElement( std::forward< Args >( args )... ) };
        std::weak_ptr< Type > wPtr = std::static_pointer_cast< Type >( shared_from_this() );
        HandleType handle{ index, wPtr };
        return ( handle );
    }

    inline const CompType* getPtr( std::size_t index ) const noexcept
    {
        return ( mComponents.getPtr( index ) );
    }

    inline CompType* getPtr( std::size_t index ) noexcept
    {
        return ( mComponents.getPtr( index ) );
    }

protected:

    util::MemoryPool< CompT > mComponents;
};

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_PROCESSOR_HPP */
