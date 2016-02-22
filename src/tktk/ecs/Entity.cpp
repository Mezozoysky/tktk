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

#include <tktk/ecs/Entity.hpp>
#include <tktk/ecs/EntityManager.hpp>

namespace tktk
{
namespace ecs
{

/*
//
// Entity
//

Entity::Entity()
: id{ 0 }
{
}

Entity::Entity ( uint32_t index, uint32_t version )
: id{ uint64_t( index ) | uint64_t( version ) << 32UL }
{

}

uint32_t Entity::index() const noexcept
{
    return ( id & 0xffffffffUL );
}

uint32_t Entity::version() const noexcept
{
    return ( id >> 32UL );
}

//
// EntityHandle
//

const Entity EntityHandle::ENTITY_INVALID{};

EntityHandle::EntityHandle( Entity entity, EntityManager* mgr )
: mEntity( entity )
, mManager( mgr )
{
}

bool EntityHandle::isValid() const noexcept
{
    return ( mManager && mManager->isEntityValid( mEntity ) );
}

void EntityHandle::invalidate() noexcept
{
    mEntity = ENTITY_INVALID;
    mManager = nullptr;
}

void EntityHandle::remove() noexcept
{
    if ( isValid() )
    {
        //TODO: rewrite it with ECS removeEntity(). it calls EntityManager destroyEntity internally but also does unmapping etc.
        mManager->destroyEntity( mEntity );
        invalidate();
    }
}
*/

} //namespace ecs
} //namespace tktk
