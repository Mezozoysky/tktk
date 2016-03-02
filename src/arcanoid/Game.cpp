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
        auto e1( mECS.addEntity() );
        auto e2( mECS.addEntity() );
        auto e3( mECS.addEntity() );

        mECS.addComp< Transform >( e1, Transform::Vector2f( 100.0f, 75.0f ) );
//         e1.addComponent< Transform >( Transform::Vector2f( 100.0f, 75.0f ) );

        mECS.addComp< Transform >( e2 );
        mECS.getComp< Transform >( e2 )->position = Transform::Vector2f( 15.0f, 100.0f );
//         e2.addComponent< Transform >();
//         e2.getComponent< Transform >().position = Transform::Vector2f( 15.0f, 100.0f );

        auto tf3( mECS.addComp< Transform >( e3 ) );
        tf3->position = Transform::Vector2f( 60.0f, 40.0f );
//         auto tf3( e3.addComponent< Transform > );
//         tf3->position = Transform::Vector2f( 60.0f, 40.0f );

        mECS.removeComp< Transform >( e2 );

        auto sprite1( mECS.addComp< StaticSprite >( e1, "texture0.png", true ) );
        auto sheet1( mECS.addComp< SpriteSheet >( e1 ) );
        sheet1->texture = "sheet_texture0.png";
        sheet1->centered = true;

//         auto sprite1( e1.addComp< StaticSprite >( "texture0.png", true  ) );
//         auto sheet1( e1.addComp< SpriteSheet >() );
//         sheet1->texture = "sheet_texture0.png";
//         sheet1->centered = true;

        mECS.removeEntity( e3 );


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

    auto transformProc( mECS.registerProcessor< TransformProc >() );
    auto staticSpriteProc( mECS.registerProcessor< StaticSpriteProc >() );
    auto spriteSheetProc( mECS.registerProcessor< SpriteSheetProc >() );
    mECS.setup();

    return ( true );
}

void Game::cleanup()
{
    SDL_DestroyRenderer( mRenderer );
    SDL_DestroyWindow( mWindow );
    SDL_Quit();
}

