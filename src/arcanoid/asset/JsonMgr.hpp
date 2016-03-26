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

#ifndef ARCANOID_ASSET_JSON_MGR_HPP
#define ARCANOID_ASSET_JSON_MGR_HPP

#include <tktk/asset/Manager.hpp>
#include <arcanoid/json/JSON.hpp>
#include <rapidjson/reader.h>
#include <rapidjson/filereadstream.h>
#include <cstdio>
#include <iostream>

using namespace tktk;

class JsonMgr
: public asset::Mgr< JSON >
{
public:
    JsonMgr( asset::System* systemPtr ) noexcept
    : asset::Mgr< JSON >( systemPtr )
    {
    }

    virtual ~JsonMgr() noexcept
    {
    }

    virtual SharedAssetTypeT loadAsset( const std::string& name ) noexcept override // TODO: Should be pure virtual
    {
        SharedAssetTypeT shared;

        std::FILE* fp{ std::fopen( name.c_str(), "r" ) };
        if ( fp == nullptr )
        {
            std::cout << "ERROR: cant open file \"" << name << "\"" << std::endl;
        }
        else
        {
            char readBuffer[256];
            rapidjson::FileReadStream input( fp, readBuffer, sizeof( readBuffer ) );

            rapidjson::Document doc;
            if ( doc.ParseStream< rapidjson::kParseDefaultFlags >( input ).HasParseError() )
            {
                //TODO: some logging lib needed
                std::cout << "ERROR: cant parse json \"" << name << "\": " << doc.GetParseError() << std::endl;
            }
            else
            {
                shared = std::make_shared< JSON >( std::move( doc ) );
            }
            std::fclose( fp );
        }
        return ( shared );
    }

private:

};

#endif // ARCANOID_ASSET_JSON_MGR_HPP