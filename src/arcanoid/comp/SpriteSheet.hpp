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

#ifndef ARCANOID_COMP_SPRITE_SHEET_HPP
#define ARCANOID_COMP_SPRITE_SHEET_HPP

#include <tktk/ecs/Component.hpp>
#include <tktk/ecs/Processor.hpp>

using namespace tktk;

struct SpriteSheet
: public ecs::Comp< SpriteSheet >
{
    explicit SpriteSheet( const util::Id64& entityId );
    virtual ~SpriteSheet();

    std::string texture{};
    bool centered{ false };
};


class SpriteSheetProc
: public ecs::Proc< SpriteSheet >
{
public:
    virtual void setup( ecs::System* systemPtr ) override;
    virtual void onUpdate( float deltaTime );
};

#endif /* end of include guard: ARCANOID_COMP_SPRITE_SHEET_HPP */
