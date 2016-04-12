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
#include "comp/RectShape.hpp"
#include "asset/TextureMgr.hpp"
#include "asset/JsonMgr.hpp"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

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
        auto texture( mAssetS.get< Texture >( "texture0.png" ) );
        auto json( mAssetS.get< JSON >( "json0.json" ) );

        if ( json != nullptr )
        {
            auto jsName( json->getDocument()[ "name" ].GetString() );
            auto jsDesc( json->getDocument()[ "description" ].GetString() );
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CONTENTS: name -> \"" << jsName << "\", description -> \"" << jsDesc << "\"" << std::endl;
        }

        auto e( mECS.addEntity() );
        auto tfPaddle( e.addComp< Transform >( Transform::Vector2f( 200.0f, 700.0f ) ) );
        e.addComp< RectShape >( 100.0f, 30.0f, SDL_Color{ 0x55, 0x55, 0xaa, 0xff } );

        e = mECS.addEntity();
        e.addComp< Transform >( Transform::Vector2f( 100.0f, 100.0f ) );
        e.addComp< StaticSprite >( texture );


        for ( int x = 0; x < 13; ++x )
        {
            for ( int y = 0; y < 4; ++y )
            {
                if ( (int)x % 2 == 0 || (int)y % 2 == 0 )
                {
                    e = mECS.addEntity();
                    e.addComp< Transform >( Transform::Vector2f( 0.0f + x * 40.0f + x * 2, 0.0f + y * 30.0f + y * 2 ) );
                    if ( (int)y % 2 == 0 )
                    {
                        e.addComp< RectShape >( 40.0f, 30.0f, SDL_Color{ 0x55, 0xaa, 0x55, 0xff } );
                        //e.addComp< HPCounter >( 1 );
                    }
                    else
                    {
                        e.addComp< RectShape >( 40.0f, 30.0f, SDL_Color{ 0xaa, 0x55, 0x55, 0xff } );
                        //e.addComp< HPCounter >( 3 );
                    }
                }
            }
        }


        mIsRunning = true;
        SDL_Event event;
        float speed{ 0.0f };
        while ( mIsRunning )
        {
            while ( SDL_PollEvent( &event ) )
            {
                switch ( event.type )
                {
                    case SDL_QUIT:
                    {
                        mIsRunning = false;
                    }
                    break;
                    case SDL_KEYDOWN:
                    {
                        switch ( event.key.keysym.sym )
                        {
                            case SDLK_LEFT:
                            {
                                speed = -5.0f;
                            }
                            break;
                            case SDLK_RIGHT:
                            {
                                speed = 5.0f;
                            }
                            break;
                            default:
                            break;
                        }
                    }
                    break;
                    case SDL_KEYUP:
                    {
                        switch ( event.key.keysym.sym )
                        {
                            case SDLK_LEFT:
                            {
                                if ( speed < 0.0f )
                                {
                                    speed = 0.0f;
                                }
                            }
                            break;
                            case SDLK_RIGHT:
                            {
                                if ( speed > 0.0f )
                                {
                                    speed = 0.0f;
                                }
                            }
                            break;
                            default:
                            break;
                        }
                    }
                    break;
                    default:
                    break;
                }
            }

//             SDL_SetRenderDrawColor( mRenderer, 0, 0, 0, 255 );
//             SDL_RenderClear( mRenderer );
// /*
            {
                SDL_Color colors[2]{
                    { 0x66, 0x66, 0x66, 0xff },
                    { 0x99, 0x99, 0x99, 0xff }
                };
                SDL_Rect rect{ 0, 0, 8, 8 };
                int i;
                for ( int y = 0; y < 768; y += rect.h)
                {
                    for ( int x = 0; x < 1024; x += rect.w)
                    {
                        i = (((x ^ y) >> 3) & 1);
                        SDL_SetRenderDrawColor(mRenderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);

                        rect.x = x;
                        rect.y = y;
                        SDL_RenderFillRect(mRenderer, &rect);
                    }
                }

            }
// */
            tfPaddle->position.x += speed;
            mECS.update( 1 /*secondsElapsed*/ );

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

    mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/ );
    if ( mRenderer == nullptr )
    {
        ll_error( "SDL_CreateRenderer Error: " << SDL_GetError() );
        return ( false );
    }

    if ( ( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) != IMG_INIT_PNG )
    {
        ll_error( "IMG_Init error: " << SDL_GetError() );
        return ( false );
    }

    mAssetS.registerMgr< TextureMgr >( mRenderer );
    mAssetS.registerMgr< JsonMgr >();

    mECS.registerProc< TransformProc >();
    mECS.registerProc< StaticSpriteProc >( mRenderer );
    mECS.registerProc< SpriteSheetProc >();
    mECS.registerProc< RectShapeProc >( mRenderer );
    mECS.setup();

    mAssetS.load< Texture >( "texture0.png" );
    mAssetS.load< JSON >( "json0.json" );

    return ( true );
}

void Game::cleanup()
{
    IMG_Quit();
    SDL_DestroyRenderer( mRenderer );
    SDL_DestroyWindow( mWindow );
    SDL_Quit();
}
