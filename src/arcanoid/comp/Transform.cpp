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

#include "Transform.hpp"
#include <tktk/ecs/System.hpp>
#include <iostream>

using namespace tktk;

Transform::Transform( const mpool::Id64& entityId, Vector2f position )
: BaseTypeT( entityId )
{
    this->position = position;
}

Transform::~Transform()
{
}

TransformProc::TransformProc(ecs::System* systemPtr)
: Proc< Transform >( systemPtr )
{
}

void TransformProc::setup()
{
    mSystemPtr->updateSignal.connect( std::bind( &TransformProc::onUpdate, this, std::placeholders::_1 ) );
}

void TransformProc::onUpdate( float deltaTime )
{
    mPool.forEach(
        [&]( Transform& transform )
        {
//             ll_debug(
//                 "Updating Transform comp;"
//                 << " position=(" << std::to_string( transform.position.x ) << ", " << std::to_string( transform.position.y ) << ")"
//             );
        }
    );
}
