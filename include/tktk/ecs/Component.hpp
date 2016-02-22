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
#include <memory>
#include <string>

namespace tktk
{
namespace ecs
{

// forward declarations
template< typename CompT >
class Processor;

struct ComponentBase
{
    ComponentBase() = default;
    virtual ~ComponentBase() = default;

    virtual const util::Id64& getEntityId() const noexcept = 0;
};

template< typename CompT >
struct Component
: public ComponentBase
{
    using Type = CompT;
    using BasalType = Component< Type >;

    explicit Component( const util::Id64& entityId ) noexcept
    : mEntityId{ entityId }
    {
    }

    virtual ~Component()
    {
    }

    virtual const util::Id64& getEntityId() const noexcept override final
    {
        return mEntityId;
    }

private:
    util::Id64 mEntityId;
};


// struct ComponentHandleBase
// {
//     bool isValid() const noexcept;
//     void invalidate() noexcept;
// };

/*
template < typename T >
struct ComponentHandle
{
    using CompType = T;

    ComponentHandle( std::size_t index, typename std::shared_ptr< ecs::Processor< T > > processor )
    : mIndex{ index }
    , mProcessor{ processor }
    {
    }

    inline bool isValid() const noexcept
    {
        return ( mProcessor && mProcessor->isComponentAlive( mIndex ) );
    }

    void invalidate() noexcept
    {
        mIndex = -1;
        mProcessor = nullptr;
    }

    inline CompType* operator ->() const noexcept
    {
        return ( mProcessor->getPtr( mIndex ) );
    }

    inline std::size_t getIndex() const noexcept
    {
        return ( mIndex );
    }


private:

    std::size_t mIndex{ 0 };
    std::shared_ptr< ecs::Processor< T > > mProcessor{ nullptr }; //TODO: it should be rather raw pointer
};
*/

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_COMPONENT_HPP */
