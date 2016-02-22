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
// : public std::enable_shared_from_this< ProcessorBase >
{
    ProcessorBase( const ProcessorBase& ) = delete;
    ProcessorBase& operator= ( const ProcessorBase& ) = delete;

public:

    ProcessorBase() = default;
    virtual ~ProcessorBase() = default;

    virtual void setup( EventProxy& eventProxy, EntityManager& entityManager ) = 0;
//     virtual void onUpdate( float deltaTime ) = 0;
//    virtual void onFixedUpdate( float deltaTime ) = 0;

//    virtual bool isComponentAlive( std::size_t index ) const noexcept = 0;
//    virtual void
};


template< typename CompT >
class Processor
: public ProcessorBase
{

public:

    using CompTypeT = CompT;
    using PoolTypeT = util::MemoryPool<CompTypeT>;

    struct Handle
    {
        Handle()
        {
        }

        Handle( const util::ElementId& eid, Processor< CompTypeT >* procPtr )
        : mElementId{ eid }
        , mProcPtr{ procPtr }
        {
        }

        inline bool isValid() const noexcept
        {
            return ( mProcPtr && mProcPtr->isElementIdValid( mElementId ) );
        }

        void invalidate() noexcept
        {
            mElementId = PoolTypeT::ElementId::INVALID;
            mProcPtr = nullptr;
        }

        inline util::ElementId getElementId() const noexcept
        {
            return ( mElementId );
        }

//         void remove() noexcept;

        inline CompTypeT* operator ->() const noexcept
        {
            if ( !isValid() )
            {
                return ( nullptr );
            }
            return ( mProcPtr->getPtr( mElementId.index() ) );
        }

    private:
        util::ElementId mElementId { util::ELEMENT_ID_INVALID };
        Processor< CompTypeT >* mProcPtr{ nullptr };
    };

    Processor()
    : ProcessorBase()
    {
    }

    virtual ~Processor() noexcept
    {
    }

//     virtual void setup( EventProxy& eventProxy, EntityManager& entityManager ) override
//     {
//         eventProxy.updateSignal.connect( std::bind( &ProcessorBase::onUpdate, this, std::placeholders::_1 ) );
//     }

    virtual bool isElementIdValid( const util::ElementId& eid ) const noexcept
    {
        return ( mPool.isElementIdValid( eid ) );
    }

    template< typename... Args >
    Handle addComponent( Args&&... args )
    {
        util::ElementId eid{ mPool.createElement( std::forward< Args >( args )... ) };

        Handle handle( eid, this );
        return ( handle );
    }

    void removeComponent( Handle& handle )
    {
        if ( handle.isValid() )
        {
            destroyElement( handle.getElementId() );
            handle.invalidate();
        }
    }

    inline void destroyElement( const util::ElementId& eid )
    {
        mPool.destroyElement( eid );
    }

    inline CompTypeT* getPtr( std::uint32_t index ) const noexcept
    {
        return ( mPool.getPtr( index ) );
    }

protected:

    PoolTypeT mPool;
};

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_PROCESSOR_HPP */
