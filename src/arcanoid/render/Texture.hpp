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

#ifndef ARCANOID_RENDER_TEXTURE_HPP
#define ARCANOID_RENDER_TEXTURE_HPP

//using namespace tktk;

#include <SDL.h>
#include <memory>

class Texture
{
public:

    using VPtr = std::shared_ptr< Texture >;

    template < typename... ArgsT >
    inline static VPtr create( ArgsT&&... args ) noexcept
    {
        VPtr vp{ std::make_shared< Texture >( std::forward< ArgsT >( args )... ) };
        return ( vp );
    }

    Texture( SDL_Texture* rawTexture ) noexcept
    : mRawTexture{ rawTexture }
    {
        query();
    }

    virtual ~Texture() noexcept
    {
        if ( mRawTexture != nullptr )
        {
            SDL_DestroyTexture( mRawTexture );
        }
    }

    inline SDL_Texture* getRawTexture() const noexcept
    {
        return ( mRawTexture );
    }

protected:

    inline bool query() noexcept
    {
        if ( mRawTexture == nullptr )
        {
            return ( false );
        }

        SDL_QueryTexture( mRawTexture, nullptr, nullptr, &mWidth, &mHeight );
        return ( true );
    }

private:
    SDL_Texture* mRawTexture{ nullptr };
    int mWidth{ 1 };
    int mHeight{ 1 };
};

#endif // ARCANOID_RENDER_TEXTURE_HPP
