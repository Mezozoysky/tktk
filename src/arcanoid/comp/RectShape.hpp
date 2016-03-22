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

#ifndef ARCANOID_COMP_RECT_SHAPE_HPP
#define ARCANOID_COMP_RECT_SHAPE_HPP

#include <tktk/ecs/Component.hpp>
#include <arcanoid/ECS.hpp>
#include <SDL.h>

using namespace tktk;


struct RectShape
: public ecs::Comp< RectShape >
{
    explicit RectShape( mpool::Id64 entityId );
    explicit RectShape( mpool::Id64 entityId, float width, float height, const SDL_Color& color, bool centered = false );
//     virtual ~RectShape();

    float width;
    float height;
    SDL_Color color{ 0x00, 0x00, 0x00, 0xFF };
    bool centered{ false };
};

class RectShapeMgr
: public CompMgr< RectShape >
{
public:
    RectShapeMgr( ECS* ecs, SDL_Renderer* renderer );
    virtual bool setup() noexcept;
    virtual void shutdown() noexcept;
    virtual void onUpdate( float deltaTime ) noexcept;

private:
    SDL_Renderer* mRenderer{ nullptr };
};

#endif /* end of include guard: ARCANOID_COMP_RECT_RENDERER_HPP */
