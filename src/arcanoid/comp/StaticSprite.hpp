/**
 *    tktk
 * 
 *    tktk - Copyright (C) 2015 Stanislav Demyanovich <stan@angrybubo.com>
 * 
 *    This software is provided 'as-is', without any express or
 *    implied warranty. In no event will the authors be held
 *    liable for any damages arising from the use of this software.
 * 
 *    Permission is granted to anyone to use this software for any purpose,
 *    including commercial applications, and to alter it and redistribute
 *    it freely, subject to the following restrictions:
 * 
 *    1.  The origin of this software must not be misrepresented;
 *        you must not claim that you wrote the original software.
 *        If you use this software in a product, an acknowledgment
 *        in the product documentation would be appreciated but
 *        is not required.
 * 
 *    2.  Altered source versions must be plainly marked as such,
 *        and must not be misrepresented as being the original software.
 * 
 *    3.  This notice may not be removed or altered from any
 *        source distribution.
 */

#ifndef ARCANOID_COMP_STATIC_SPRITE_HPP
#define ARCANOID_COMP_STATIC_SPRITE_HPP

#include <tktk/ecs/Component.hpp>
#include <tktk/ecs/Processor.hpp>
#include <arcanoid/render/Texture.hpp>
#include <SDL.h>

using namespace tktk;


struct StaticSprite
: public ecs::Comp< StaticSprite >
{
    explicit StaticSprite( const mpool::Id64& entityId );
    explicit StaticSprite( const mpool::Id64& entityId, Texture::VPtr texture, bool centered = false );
    virtual ~StaticSprite();

    Texture::VPtr texture{ nullptr };
    bool centered{ false };
};


class StaticSpriteProc
: public ecs::Proc< StaticSprite >
{
public:
    StaticSpriteProc( ecs::System* systemPtr, SDL_Renderer* renderer );
    virtual void setup() override;
    virtual void onUpdate( float deltaTime );

private:
    SDL_Renderer* mRenderer{ nullptr };
};

#endif /* end of include guard: ARCANOID_COMP_STATIC_SPRITE_HPP */
