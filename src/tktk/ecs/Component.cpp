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

#include <tktk/ecs/Component.hpp>
#include <tktk/ecs/Processor.hpp>

namespace tktk
{
namespace ecs
{

//
// Component
//

Component::Component( const mpool::Id64& entityId ) noexcept
: mEntityId{ entityId }
{
}

Component::~Component() noexcept
{
}


//
// Component::Handle
//

Component::Handle::Handle() noexcept
{
}

Component::Handle::Handle( const mpool::Id64& cId, Processor* procPtr ) noexcept
: mId{ cId }
, mProcPtr{ procPtr }
{
}

bool Component::Handle::isValid() const noexcept
{
    return ( mProcPtr && mProcPtr->isIdValid( mId ) );
}

void Component::Handle::invalidate() noexcept
{
    mId = mpool::ID64_INVALID;
    mProcPtr = nullptr;
}

Component* Component::Handle::operator ->() const noexcept
{
    if ( !isValid() )
    {
        return ( nullptr );
    }
    return ( mProcPtr->getCompPtr( mId ) );
}


} //namespace ecs
} //namespace tktk
