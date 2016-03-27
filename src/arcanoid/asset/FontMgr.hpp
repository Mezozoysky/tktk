//  tktk
//
//  tktk - Copyright (C) 2016 Stanislav Demyanovich <stan@angrybubo.com>
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

#ifndef ARCANOID_ASSET_FONT_MGR_HPP
#define ARCANOID_ASSET_FONT_MGR_HPP

#include <tktk/asset/Manager.hpp>
#include <arcanoid/render/Font.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
#include <cassert>

using namespace tktk;

class FontMgr
: public asset::Mgr< Font >
{
public:
    FontMgr( asset::System* systemPtr, SDL_Renderer* rendererPtr ) noexcept
    : asset::Mgr< Font >( systemPtr )
    , mRenderPtr{ rendererPtr }
    {
    }

    virtual ~FontMgr() noexcept
    {
    }

    virtual SharedAssetTypeT loadAsset( const std::string& name ) noexcept override // TODO: Should be pure virtual
    {
        SharedAssetTypeT shared;

        auto rit( name.rbegin() );
        while ( rit != name.rend() )
        {
            if ( *rit == ':' )
            {
                break;
            }
            ++rit;
        }
        if ( rit == name.rend() )
        {
            assert( false && "FontMgr can't parse willing font ptsize from asset name" );
        }
        auto fit = --( rit.base() ); //forward iterator

        std::string fileName{ name.begin(), fit };
        std::string ptStr{ ++fit, name.end() };
        int ptSize{ std::stoi( ptStr ) };

        TTF_Font* rawFont{ TTF_OpenFontIndex( fileName.c_str(), ptSize, 0 ) };
        if ( rawFont == nullptr )
        {
            //TODO: some logging lib needed
        }
        else
        {
            shared = Font::create( rawFont, ptSize );
        }
        return ( shared );
    }

private:
    SDL_Renderer* mRenderPtr{ nullptr };
};

#endif // ARCANOID_ASSET_FONT_MGR_HPP
