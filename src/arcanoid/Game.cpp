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

#include "Game.hpp"
#include "comp/Transform.hpp"
#include "comp/StaticSprite.hpp"
#include "comp/SpriteSheet.hpp"
#include <iostream>
#include <type_traits>
#include <SDL.h>

using namespace tktk;

Game::Game()
: mIsRunning{ false }
, mWindow{ nullptr }
, mRenderer{ nullptr }
{
}

Game::~Game()
{
}

void Game::run()
{
    if ( setup() )
    {
        //
        // entity-centric api test
        //
        auto e0( mECS.addEntity() );
        e0.addComp< Transform >( Transform::Vector2f( 0.5f, 0.5f ) );
        e0.addComp< StaticSprite >( "e0sprite.png", 1 );
        //e0.removeComp< StaticSprite >();

        auto e1( mECS.addEntity() );
        auto t1( e1.addComp< Transform >() );
        t1->position = Transform::Vector2f( 1.0f, 1.0f );
        auto ss1( e1.addComp< StaticSprite >() );
        ss1->texture = "ss1.png";

        auto e2( mECS.addEntity() );
        e2.addComp< Transform >();
        auto t2( e2.getComp< Transform >() );
        t2->position = Transform::Vector2f( 2.0f, 2.0f );
        e2.addComp< StaticSprite >();
        auto ss2( e2.getComp< StaticSprite >() );
        ss2->centered = true;
        ss2->texture = "ss2.png";

        //
        // system-centric api test
        //
        auto e3( mECS.addEntity() );
        mECS.addComp< StaticSprite >( e3, "e3sprite.png", 0 );

        auto e4( mECS.addEntity() );
        auto ss4( mECS.addComp< StaticSprite >( e4 ) );
        ss4->centered = 1;
        ss4->texture = "ss4texture.png";

        auto e5( mECS.addEntity() );
        mECS.addComp< StaticSprite >( e5 );
        auto ss5( mECS.getComp< StaticSprite >( e5 ) );
        ss5->texture = "ss5.png";

        mIsRunning = true;
        SDL_Event event;
        while ( mIsRunning )
        {
            while ( SDL_PollEvent( &event ) )
            {
                if ( event.type == SDL_QUIT )
                {
                    mIsRunning = false;
                }
            }

            mECS.update( 1 /*secondsElapsed*/ );
            SDL_RenderClear( mRenderer );
            SDL_RenderPresent( mRenderer );
            //std::cout << "FPS: " << std::to_string( 1.0f / secondsElapsed ) << std::endl;
        }
    }
    cleanup();
}

bool Game::setup()
{
    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
        ll_error( "SDL_Init ERROR: " << SDL_GetError() );
        return ( false );
    }

    mWindow = SDL_CreateWindow( "Hello!", 100, 100, 1024, 768, SDL_WINDOW_SHOWN );
    if ( mWindow == nullptr )
    {
        ll_error( "SDL_CreateWindow ERROR: " << SDL_GetError() );
        return ( false );
    }

    mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if ( mRenderer == nullptr )
    {
        ll_error( "SDL_CreateRenderer Error: " << SDL_GetError() );
        return ( false );
    }

    auto transformProc( mECS.registerProc< TransformProc >() );
    auto staticSpriteProc( mECS.registerProc< StaticSpriteProc >() );
    auto spriteSheetProc( mECS.registerProc< SpriteSheetProc >() );
    mECS.setup();

    return ( true );
}

void Game::cleanup()
{
    SDL_DestroyRenderer( mRenderer );
    SDL_DestroyWindow( mWindow );
    SDL_Quit();
}

