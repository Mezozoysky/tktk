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
#include <tktk/ecs/System.hpp>

using namespace tktk;

RectShape::RectShape( const util::Id64& entityId )
: BaseTypeT( entityId )
{
}

RectShape::RectShape(const util::Id64& entityId, float width, float height, const SDL_Color& color, bool centered)
: Comp< RectShape >(entityId)
, width{ width }
, height{ height }
, color( color )
, centered{ centered }
{
}

RectShape::~RectShape()
{
}

RectShapeProc::RectShapeProc( ecs::System* systemPtr, SDL_Renderer* renderer )
: Proc< RectShape >( systemPtr )
, mRenderer{ renderer }
{
}

void RectShapeProc::setup( ecs::System* systemPtr )
{
    systemPtr->updateSignal.connect( std::bind( &RectShapeProc::onUpdate, this, std::placeholders::_1 ) );
}

void RectShapeProc::onUpdate( float deltaTime )
{
    for ( int i{ 0 }; i < mPool.getSize(); ++i )
    {
        if ( !mPool.isAlive( i ) ) continue;
        auto comp = mPool[ i ];
//         ll_debug( "Updating RectShape comp#" << std::to_string( i ) << " width=" << comp.width << ", height=" << comp.height << ", centered=" << std::to_string( comp.centered ) );

        auto t( mSystemPtr->getComp< Transform >( comp.getEntityId() ) );
        SDL_Rect rect;
        rect.w = comp.width;
        rect.h = comp.height;
        rect.x = t->position.x;
        rect.y = t->position.y;
        SDL_SetRenderDrawColor( mRenderer, comp.color.r, comp.color.g, comp.color.b, comp.color.a );
        SDL_RenderFillRect( mRenderer, &rect );
    }
}
