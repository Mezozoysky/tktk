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

#include "Image.hpp"
#include "Transform.hpp"
#include <arcanoid/ECS.hpp>

using namespace tktk;

Image::Image(mpool::Id64 eId ) noexcept
: BaseTypeT( eId )
{
}

Image::Image( mpool::Id64 eId, Texture::VPtr texture, bool centered ) noexcept
: BaseTypeT( eId )
, texture{ texture }
, centered{ centered }
{
}

// StaticSprite::~StaticSprite()
// {
// }

ImageMgr::ImageMgr( ECS* ecs, SDL_Renderer* renderer )
: CompMgr< Image >( ecs )
, mRenderer{ renderer }
{
}

bool ImageMgr::setup() noexcept
{
    auto ecs( getECS() );
    assert( ecs != nullptr && "GRIEEEEF" );
    ecs->updateSignal.connect( std::bind( &ImageMgr::onUpdate, this, std::placeholders::_1 ) );
    return ( true );
}

void ImageMgr::shutdown() noexcept
{
}

void ImageMgr::onUpdate( float deltaTime ) noexcept
{
    forEach(
        [&] ( Image& im )
        {
            auto texture( im.texture );
            if ( !texture )
            {
                ll_error( "StaticSprite update: empty texture!" );
                return;
            }
            int w, h;
            auto rawTexture( texture->getRawTexture() );
            SDL_QueryTexture( rawTexture, NULL, NULL, &w, &h );
            SDL_Rect srcRect{ 0, 0, w, h };
            auto t( getECS()->getComp< Transform >( im.getEntityId() ) );
            SDL_Rect dstRect{ int( t->position.x ), int( t->position.y ), w, h };
            SDL_RenderCopy( mRenderer, rawTexture, &srcRect, &dstRect);
//             ll_debug( "Updating StaticSprite comp;"
//                 << " texture=" << ss.texture
//                 << ", centered=" << std::to_string( ss.centered )
//             );
        }
    );
}
