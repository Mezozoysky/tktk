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

#ifndef TKTK_ECS_COMPONENT_HPP
#define TKTK_ECS_COMPONENT_HPP

#include <tktk/ecs/Entity.hpp>
//#include <tktk/ecs/Processor.hpp>
#include <string>

namespace tktk
{
namespace ecs
{

class ProcessorBase;

struct ComponentBase
{

public:
    ComponentBase() = default;
    virtual ~ComponentBase() = default;

    virtual const Entity& getEntity() const noexcept = 0;

private:

    Entity mOwner;
};

template< typename CompT >
struct Component
: public ComponentBase
{
    using Type = CompT;
    using BasalType = Component< Type >;

    explicit Component( const Entity& owner ) noexcept
    : mOwner{ owner }
    {
    }

    explicit Component( const EntityHandle& ownerHandle) noexcept
    : mOwner{ ownerHandle.getEntity() }
    {
    }

    virtual const Entity& getEntity() const noexcept override final
    {
        return mOwner;
    }

private:
    Entity mOwner;
};

template< typename ProcT >
struct ComponentHandle
{
    using ProcType = typename ProcT::Type;
    using CompType = typename ProcT::CompType;

    ComponentHandle( std::size_t index, std::weak_ptr< ProcType > processor )
    : mIndex{ index }
    , mProcessor{ processor }
    {
    }

    inline CompType* operator ->()
    {
//        isValid();
        auto proc = mProcessor.lock();
        if ( !proc )
        {
            return ( nullptr );
        }
        return ( proc->getPtr( mIndex ) );
    }

private:
    std::size_t mIndex;
    std::weak_ptr< ProcType > mProcessor;
};

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_COMPONENT_HPP */
