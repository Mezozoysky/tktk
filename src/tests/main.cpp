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

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <tktk/util/TypeMap.hpp>
#include <tktk/util/MemoryPool.hpp>
#include <string>

using namespace tktk;

TEST_CASE( "TypeMap correctness", "[typemap]" )
{
    using Value = std::string;

    util::TypeMap< Value > typeMap;
    typeMap.insert< int >( "integer type" );

    CHECK( typeMap.find< int >()->second == "integer type" );
}

TEST_CASE( "MemoryPool correctness", "[mempool]" )
{
    using Value = std::string;

    util::MemoryPool< Value, 2 > pool;

    CHECK( pool.getCapacity() == 0 );
    CHECK( pool.getSize() == 0 );

    std::size_t index{ pool.createElement( "zero" ) };

    CHECK( pool.getCapacity() == 2 );
    CHECK( pool.getSize() == 1 );
    CHECK( pool[ index ] == "zero" );

    index = pool.createElement( "one" );

    CHECK( pool.getCapacity() == 2 );
    CHECK( pool.getSize() == 2 );
    CHECK( pool[ index ] == "one" );

    index = pool.createElement( "two" );

    CHECK( pool.getCapacity() == 4 );
    CHECK( pool.getSize() == 3 );
    CHECK( pool[ index ] == "two" );

    index = pool.createElement( "three" );

    CHECK( pool.getCapacity() == 4 );
    CHECK( pool.getSize() == 4 );
    CHECK( pool[ index ] == "three" );

    index = pool.createElement( "four" );

    CHECK( pool.getCapacity() == 6 );
    CHECK( pool.getSize() == 5 );
    CHECK( pool[ index ] == "four" );
}
