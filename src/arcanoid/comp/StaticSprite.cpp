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

#include "StaticSprite.hpp"
#include <iostream>
#include <tktk/ecs/System.hpp>

using namespace tktk;

StaticSprite::StaticSprite( const util::Id64& entityId )
: BasalType( entityId )
{
}

StaticSprite::StaticSprite( const util::Id64& entityId, const std::string& texture, bool centered )
: BasalType( entityId )
, texture{ texture }
, centered{ centered }
{
}

StaticSprite::~StaticSprite()
{
}

void StaticSpriteProcessor::setup( ecs::System* systemPtr )
{
    systemPtr->updateSignal.connect( std::bind( &StaticSpriteProcessor::onUpdate, this, std::placeholders::_1 ) );
}

void StaticSpriteProcessor::onUpdate( float deltaTime )
{
    std::cout << "STATIC SPRITE PROCESSOR Delta time: " << std::to_string( deltaTime ) << "; Updating: " << std::endl;
    for ( int i{ 0 }; i < mPool.getSize(); ++i )
    {
        if ( !mPool.isAlive( i ) ) continue;
        auto comp = mPool[ i ];
        std::cout << std::to_string( i ) << " texture=" << comp.texture << ", centered=" << std::to_string( comp.centered ) << std::endl;
    }
}
