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

#ifndef ARCANOID_COMP_TRANSFORM_HPP
#define ARCANOID_COMP_TRANSFORM_HPP

#include <tktk/ecs/Component.hpp>
#include <tktk/ecs/Processor.hpp>

using namespace tktk;


struct Transform
: public ecs::Comp< Transform >
{
    struct Vector2f
    {
        float x{ 0.0f };
        float y{ 0.0f };

        Vector2f()
        {
        }

        Vector2f( float x, float y )
        : x{ x }
        , y{ y }
        {
        }
    };
    explicit Transform( const util::Id64& entityId, Vector2f position = Vector2f{} );
    virtual ~Transform();

    Vector2f position{};
};


class TransformProc
: public ecs::Proc< Transform >
{
public:

    virtual void setup( ecs::System* systemPtr ) override;
    virtual void onUpdate( float deltaTime );
};

#endif /* end of include guard: ARCANOID_COMP_TRANSFORM_HPP */
