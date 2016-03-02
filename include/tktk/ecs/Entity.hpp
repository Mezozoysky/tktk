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

#include <tktk/Config.hpp>
#include <cstdint>
#include <tktk/util/TypeMap.hpp>
#include <tktk/util/MemoryPool.hpp>
#include <tktk/ecs/Component.hpp>

namespace tktk
{
namespace ecs
{

//Forward declarations
class System;

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
        /// Constructor
        Handle( const util::Id64& eid, System* mgrPtr );

        /// 'Destroys' the entity, invalidates the handle itself
        void remove() noexcept;
        /// Returns the raw pointer to the entity
        Entity* operator ->() const noexcept;

        /// Adds the component of given type T constructed with given arguments args to th entity
//         template< typename T, typename... ArgsT >
//         typename T::Handle addComp( ArgsT&&... args )
//         {
//             return ( mSystemPtr->addComp< T >( mId, std::forward< ArgsT >( args )... ) );
//         }

        /// Returns the handle for entity's component of the given type T if added
//         template< typename T >
//         typename T::Handle getComp() noexcept
//         {
//             return ( mSystemPtr->getComp< T >() );
//         }

        /// Removes the component of the given type T from the entity if added
//         template< typename T >
//         void removeComp() noexcept
//         {
//             mSystemPtr->removeComp< T >();
//         }

        /// Returns entity's id
        inline util::Id64 getId() const noexcept
        {
            return ( mId );
        }
        /// Returns pointer to the System instance which created the handle
        inline System* getSystem() const noexcept
        {
            return ( mSystemPtr );
        }
        /// Returns true if handle is valid, othervise returns false
        bool isValid() const noexcept;
        /// Makes handle invalid
        void invalidate() noexcept;



    private:
        util::Id64 mId{ util::ID64_INVALID };
        System* mSystemPtr{ nullptr };
    };


    util::TypeMap< Component::Handle > map; //TODO: make if more efficient and more friendly
};


} //namespace ecs
} //namespace tktk

#endif /* end of include guard: TKTK_ECS_ENTITY_HPP */
