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

#include <tktk/ecs/EntityManager.hpp>
#include <tktk/ecs/Entity.hpp>

#include <iostream>

namespace tktk
{
namespace ecs
{

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

EntityHandle EntityManager::createEntity() noexcept
{
    std::uint32_t index{ mNextEntityIndex++ };
    mEntityVersions.resize( mNextEntityIndex );
    std::uint32_t version{ 1 };
    mEntityVersions[ index ] = version;

    EntityHandle handle{ { index, version}, this };
    std::cout << "Entity created: " << handle.getEntity().id << " [ " << index << ", " << version << " ];" << std::endl;
    return ( handle );
}

void EntityManager::destroyEntity ( Entity entity ) noexcept
{
    if ( !isEntityValid( entity ) )
    {
        return;
    }

    std::uint32_t index{ entity.index() };
    mEntityVersions[ index ]++;
    //TODO: mark index as free here!
}

bool EntityManager::isEntityValid( Entity entity ) const noexcept
{
    return ( entity.index() < mEntityVersions.size() && mEntityVersions[ entity.index() ] == entity.version() );
}


} //namespace ecs
} //namespace tktk
