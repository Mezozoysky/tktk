//  tktk
//
//  tktk - Copyright (C) 2015 Stanislav Demyanovich <stan@angrybubo.com>
//
//  This software is provided 'as-is', without any express or
//  implied warranty. In no event will the authors be held
//  liable for any damages arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute
//  it freely, subject to the following restrictions:
// 
//  1.  The origin of this software must not be misrepresented;
//      you must not claim that you wrote the original software.
//      If you use this software in a product, an acknowledgment
//      in the product documentation would be appreciated but
//      is not required.
//
//  2.  Altered source versions must be plainly marked as such,
//      and must not be misrepresented as being the original software.
//
//  3.  This notice may not be removed or altered from any
//      source distribution.


#include "RectShape.hpp"
#include "Transform.hpp"
#include <cassert>

using namespace tktk;

RectShape::RectShape( mpool::Id64 entityId )
: BaseTypeT( entityId )
{
}

RectShape::RectShape(mpool::Id64 entityId, float width, float height, const SDL_Color& color, bool centered)
: Comp< RectShape >(entityId)
, width{ width }
, height{ height }
, color( color )
, centered{ centered }
{
}

// RectShape::~RectShape()
// {
// }

RectShapeMgr::RectShapeMgr( ECS* ecs, SDL_Renderer* renderer )
: CompMgr< RectShape >( ecs )
, mRenderer{ renderer }
{
}

bool RectShapeMgr::setup() noexcept
{
    auto ecs( getECS() );
    assert( ecs != nullptr && "GRIEEEEF" );
    ecs->updateSignal.connect( std::bind( &RectShapeMgr::onUpdate, this, std::placeholders::_1 ) );
    return ( true );
}

void RectShapeMgr::shutdown() noexcept
{
}

void RectShapeMgr::onUpdate( float deltaTime ) noexcept
{
    forEach(
        [&] ( RectShape& shape )
        {
            auto t( getECS()->getComp< Transform >( shape.getEntityId() ) );
            SDL_Rect rect;
            rect.w = shape.width;
            rect.h = shape.height;
            rect.x = t->position.x;
            rect.y = t->position.y;
            SDL_SetRenderDrawColor( mRenderer, shape.color.r, shape.color.g, shape.color.b, shape.color.a );
            SDL_RenderFillRect( mRenderer, &rect );
//             ll_debug( "Updating RectShape comp;"
//                 << " width=" << shape.width
//                 << ", height=" << shape.height
//                 << ", centered=" << std::to_string( shape.centered )
//             );
        }
    );
}
