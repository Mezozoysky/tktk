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

#ifndef ARCANOID_ASSET_TEXTURE_MGR_HPP
#define ARCANOID_ASSET_TEXTURE_MGR_HPP

#include <tktk/asset/Manager.hpp>
#include <arcanoid/render/Texture.hpp>
#include <SDL.h>
#include <SDL_image.h>

using namespace tktk;

class TextureMgr
: public asset::Mgr< Texture >
{
public:
    TextureMgr( asset::System* systemPtr, SDL_Renderer* rendererPtr ) noexcept
    : asset::Mgr< Texture >( systemPtr )
    , mRenderPtr{ rendererPtr }
    {
    }

    virtual ~TextureMgr() noexcept
    {
    }

    virtual SharedAssetTypeT loadAsset( const std::string& name ) noexcept override // TODO: Should be pure virtual
    {
        SharedAssetTypeT shared;

        SDL_Texture* rawTexture{ IMG_LoadTexture( mRenderPtr, name.c_str() ) };
        if ( !rawTexture )
        {
            //TODO: some logging lib needed
        }
        else
        {
            shared = std::make_shared< Texture >( rawTexture );
        }
        return ( shared );
    }

private:
    SDL_Renderer* mRenderPtr{ nullptr };
};

#endif // ARCANOID_ASSET_TEXTURE_MGR_HPP