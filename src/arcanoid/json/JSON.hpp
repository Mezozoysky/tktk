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

#ifndef ARCANOID_JSON_JSON_HPP
#define ARCANOID_JSON_JSON_HPP

//using namespace tktk;

#include <rapidjson/document.h>
#include <memory>

using namespace rapidjson;

class JSON
{
public:

    using VPtr = std::shared_ptr< JSON >;

    JSON( Document document ) noexcept
    {
        mDocument = std::move( document );
    }

    virtual ~JSON() noexcept
    {
    }

    inline Document& getDocument() noexcept
    {
        return ( mDocument );
    }

    inline const Document& getDocument() const noexcept
    {
        return ( mDocument );
    }

private:
    Document mDocument;
};

#endif // ARCANOID_JSON_JSON_HPP
