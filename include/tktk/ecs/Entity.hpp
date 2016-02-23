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
#include <tktk/util/TypeMap.hpp>
#include <tktk/util/MemoryPool.hpp>

namespace tktk
{
namespace ecs
{

//Forward declarations
class EntityManager;

///
/// Entity
///
struct Entity
{
    ///
    /// Entity::Handle
    ///
    struct Handle
    {
        Handle( const util::Id64& eid, EntityManager* mgrPtr );
        bool isValid() const noexcept;
        void invalidate() noexcept;
        inline util::Id64 getId() const noexcept
        {
            return ( mId );
        }
        void remove() noexcept;
        Entity* operator ->() const noexcept;

    private:
        util::Id64 mId{ util::ID64_INVALID };
        EntityManager* mMgrPtr{ nullptr };
    };


    util::TypeMap< util::Id64 > map; //TODO: make if more efficient and more friendly
};


} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_ENTITY_HPP */
