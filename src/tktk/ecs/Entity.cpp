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
#include <tktk/ecs/System.hpp>

namespace tktk
{
namespace ecs
{

Entity::Handle::Handle( const util::Id64& eid, System* systemPtr )
: mId{ eid }
, mSystemPtr{ systemPtr }
{
}

Entity* Entity::Handle::operator ->() const noexcept
{
    if ( !isValid() )
    {
        return ( nullptr );
    }
    return ( mSystemPtr->getEntityPtr( mId ) );
}

void Entity::Handle::remove() noexcept
{
    if ( isValid() )
    {
        mSystemPtr->removeEntity( *this );
    }
}

bool Entity::Handle::isValid() const noexcept
{
    return ( mSystemPtr && mSystemPtr->isIdValid( mId ) );
}

void Entity::Handle::invalidate() noexcept
{
    mId = util::ID64_INVALID;
    mSystemPtr = nullptr;
}


} //namespace ecs
} //namespace tktk
