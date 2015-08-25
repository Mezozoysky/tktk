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

#ifndef TKTK_ECS_ENTITY_HPP
#define TKTK_ECS_ENTITY_HPP

#include <cstdint>


namespace tktk
{
namespace ecs
{

class EntityManager;

struct Entity
{
    Entity();
    Entity( uint32_t index, uint32_t version );

    uint32_t index() const noexcept;
    uint32_t version() const noexcept;

    uint64_t id{ 0 };
};

struct EntityHandle
{
    static const Entity ENTITY_INVALID;

    EntityHandle( Entity entity, EntityManager* mgr );

    inline bool isValid() const noexcept;
    void invalidate() noexcept;

    inline Entity getEntity() const noexcept
    {
        return ( mEntity );
    }
    void destroyEntity() noexcept;

private:

    Entity mEntity { ENTITY_INVALID };
    EntityManager* mManager;
};

} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_ENTITY_HPP */
