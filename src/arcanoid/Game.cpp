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
#include "asset/FontMgr.hpp"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace tktk;

Game::Game()
: mIsRunning{ false }
, mWindow{ nullptr }
, mRenderer{ nullptr }
, mWindowWidth{ 800 }
, mWindowHeight{ 600 }
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

        auto font( mAssetS.get< Font >( "pt_mono_regular.ttf:42" ) );

        if ( font != nullptr )
        {
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! we have font!" << std::endl;
        }

        auto e( mECS.addEntity() );
        auto tfPaddle( e.addComp< Transform >( Transform::Vector2f( ( mWindowWidth - 128.0f ) / 2.0f, mWindowHeight - 44.0f ) ) );
        e.addComp< RectShape >( 128.0f, 24.0f, SDL_Color{ 0x3f, 0x7f, 0xff, 0xff } );

        e = mECS.addEntity();
        e.addComp< Transform >( Transform::Vector2f( ( mWindowWidth - 30.0f ) / 2.0f, ( mWindowHeight - 30.0f ) / 2.0f ) );
        e.addComp< StaticSprite >( texture );

        int cols{ 13 };
        int rows{ 4 };
        float brickW{ 59.0f };
        float brickH{ 24.0f };
        float marginW{ 2 };
        float marginH{ 4 };
        float totalW{ cols * ( brickW + marginW ) + marginW };
//         float totalH{ rows * ( brickH + marginH ) + marginH };
        float offsetX{ (int)(( mWindowWidth - totalW ) / 2.0f + 0.5f) + marginW };
        float offsetY{ marginH };
        for ( int x = 0; x < cols; ++x )
        {
            for ( int y = 0; y < rows; ++y )
            {
                if ( (int)x % 2 == 0 || (int)y % 2 == 0 )
                {
                    e = mECS.addEntity();
                    e.addComp< Transform >( Transform::Vector2f( offsetX + x * ( brickW + marginW ), offsetY + y * ( brickH + marginH ) ) );

                    if ( (int)y % 2 == 0 )
                    {
                        e.addComp< RectShape >( brickW, brickH, SDL_Color{ 0x3f, 0xff, 0x7f, 0xff } );
                        //e.addComp< HPCounter >( 1 );
                    }
                    else
                    {
                        e.addComp< RectShape >( brickW, brickH, SDL_Color{ 0xff, 0x3f, 0x7f, 0xff } );
                        //e.addComp< HPCounter >( 3 );
                    }
                }
            }
        }


        float speed{ 0.0f };
        mIsRunning = true;
        SDL_Event event;
        std::uint32_t frameCount{ 0 };
        std::uint32_t msecElapsed{ 0 };
        double secElapsed{ 0.0 };
        double fps{ 0 };
        std::uint32_t msecStart{ SDL_GetTicks() };
        while ( mIsRunning )
        {
            ++frameCount;
            msecElapsed = SDL_GetTicks() - msecStart;
            secElapsed = msecElapsed / 1000.0;
            fps = frameCount / secElapsed;
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
                            case SDLK_ESCAPE:
                            {
                                mIsRunning = false;
                            }
                            break;
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
                            {
                            }
                            break;
                        }
                    }
                    break;
                    case SDL_WINDOWEVENT:
                    {
                        switch ( event.window.event )
                        {
                        case SDL_WINDOWEVENT_RESIZED:
                        {
                            SDL_GetWindowSize( mWindow, &mWindowWidth, &mWindowHeight );
                            std::cout << "RESIZED!" << std::endl;
                        }
                        break;
                        default:
                        {
                        }
                        break;
                        };
                    }
                    break;
                    default:
                    {
                    }
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
                for ( int y = 0; y < mWindowHeight; y += rect.h)
                {
                    for ( int x = 0; x < mWindowWidth; x += rect.w)
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

            mECS.updateSignal( secElapsed );

            SDL_Surface* fpsSurface{ TTF_RenderUTF8_Blended( font->getRawFont(), std::to_string( fps ).c_str() , { 0xff, 0xff, 0x7f, 0xff } ) };
            SDL_Texture* fpsTexture{ SDL_CreateTextureFromSurface( mRenderer, fpsSurface ) };
            SDL_SetTextureAlphaMod( fpsTexture, 0x9f );
            SDL_FreeSurface( fpsSurface );
            SDL_Rect srcRect{ 0, 0, 0, 0 };
            SDL_QueryTexture( fpsTexture, NULL, NULL, &(srcRect.w), &(srcRect.h) );
            SDL_Rect dstRect{ mWindowWidth - srcRect.w, mWindowHeight - srcRect.h, srcRect.w, srcRect.h };
            SDL_RenderCopy(mRenderer, fpsTexture, &srcRect, &dstRect );
            SDL_DestroyTexture( fpsTexture );
            SDL_RenderPresent( mRenderer );
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

    mWindow = SDL_CreateWindow( "Hello!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    if ( mWindow == nullptr )
    {
        ll_error( "SDL_CreateWindow ERROR: " << SDL_GetError() );
        return ( false );
    }

    SDL_GetWindowSize( mWindow, &mWindowWidth, &mWindowHeight );

    mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/ );

    if ( mRenderer == nullptr )
    {
        ll_error( "SDL_CreateRenderer Error: " << SDL_GetError() );
        return ( false );
    }

    if ( ( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) != IMG_INIT_PNG )
    {
        ll_error( "IMG_Init error: " << IMG_GetError() );
        return ( false );
    }

    if ( TTF_Init() == -1 )
    {
        ll_error( "TTF_Init error: " << TTF_GetError() );
        return ( false );
    }

    mAssetS.registerMgr< TextureMgr >( mRenderer );
    mAssetS.registerMgr< JsonMgr >();
    mAssetS.registerMgr< FontMgr >( mRenderer );

    mECS.registerProc< TransformProc >();
    mECS.registerProc< StaticSpriteProc >( mRenderer );
    mECS.registerProc< SpriteSheetProc >();
    mECS.registerProc< RectShapeProc >( mRenderer );
    mECS.setup();

    mAssetS.load< Texture >( "texture0.png" );
    mAssetS.load< JSON >( "json0.json" );
    mAssetS.load< Font >( "pt_mono_regular.ttf:42" );

    return ( true );
}

void Game::cleanup()
{
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer( mRenderer );
    SDL_DestroyWindow( mWindow );
    SDL_Quit();
}
