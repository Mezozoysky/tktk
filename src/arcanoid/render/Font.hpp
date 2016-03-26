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

#ifndef ARCANOID_RENDER_FONT_HPP
#define ARCANOID_RENDER_FONT_HPP

//using namespace tktk;

#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>

class Font
{
public:

    using VPtr = std::shared_ptr< Font >;

    template < typename... ArgsT >
    inline static VPtr create( ArgsT&&... args ) noexcept
    {
        VPtr vp{ std::make_shared< Font >( std::forward< ArgsT >( args )... ) };
        return ( vp );
    }

    Font( TTF_Font* rawFont, int ptSize ) noexcept
    : mRawFont{ rawFont }
    , mPtSize{ ptSize }
    {
    }

    virtual ~Font() noexcept
    {
        if ( mRawFont != nullptr )
        {
            TTF_CloseFont( mRawFont );
        }
    }

    inline TTF_Font* getRawFont() const noexcept
    {
        return ( mRawFont );
    }

private:
    TTF_Font* mRawFont{ nullptr };
    int mPtSize;
};

#endif // ARCANOID_RENDER_FONT_HPP
