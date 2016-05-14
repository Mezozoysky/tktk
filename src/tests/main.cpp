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
#include <Catch/catch.hpp>

#include <tktk/typec/TypeMap.hpp>
#include <tktk/typec/TypeMask.hpp>
#include <tktk/mpool/MemoryPool.hpp>
#include <tktk/util/Signal.hpp>
#include <tktk/ecs/System.hpp>
#include <string>
#include <iostream>

using namespace tktk;

/*
TEST_CASE( "Vector TypeMap correctness", "[typemap]" )
{
    using Value = std::string;

    typemap::TypeMap< Value > typeMap( "" );
    CHECK( typeMap.getInvalid() == "" );
    CHECK( typeMap.getSize() == 0 );
    CHECK( typeMap.getElementsCount() == 0 );

    typeMap.set< int >( "integer type" );
    CHECK( typeMap.get< int >() == "integer type" );
    CHECK( typeMap.getSize() == 1 );
    CHECK( typeMap.getElementsCount() == 1 );

    typeMap.set< long >( "long integer type" );
    CHECK( typeMap.get< long >() == "long integer type" );
    CHECK( typeMap.getSize() == 2 );
    CHECK( typeMap.getElementsCount() == 2 );

    CHECK( typeMap.get< std::string >() == typeMap.getInvalid() );
    CHECK( typeMap.getSize() == 3 );
    CHECK( typeMap.getElementsCount() == 2 );

    typeMap.set< std::string >( "std library string" );
    CHECK( typeMap.get< std::string >() == "std library string" );
    CHECK( typeMap.getSize() == 3 );
    CHECK( typeMap.getElementsCount() == 3 );

    typeMap.erase< long >();
    CHECK( typeMap.get< long >() == typeMap.getInvalid() );
    CHECK( typeMap.getSize() == 3 );
    CHECK( typeMap.getElementsCount() == 2 );

}
*/

TEST_CASE( "TypeMap correctness", "[typemap]" )
{
    using Value = std::string;

    typec::TypeMap< Value > typeMap;
    typeMap.insert< int >( "integer type" );

    CHECK( typeMap.find< int >()->second == "integer type" );
}

TEST_CASE( "TypeMask correctness", "[typemask]" )
{
    typec::TypeMask< 2 > typeSet;
    typeSet.reset().set< int >().set< std::string >();

    CHECK( typeSet.test< int >() == true );
    CHECK( typeSet.test< std::string >() == true );
    typeSet.reset< int >();
    CHECK( typeSet.test< int >() == false );

    typec::TypeMask< 2 > typeSet2;
    typeSet2.reset().set< unsigned long >().set< char[] >();

    CHECK( typeSet2.test< unsigned long >() == true );
    CHECK( typeSet2.test< char[] >() == true );
    typeSet2.reset< unsigned long >();
    CHECK( typeSet2.test< unsigned long >() == false );
}

TEST_CASE( "MemoryPool correctness", "[mempool]" )
{
    using Value = std::string;

    mpool::MemoryPool< Value, 2 > pool;

    CHECK( pool.getCapacity() == 0 );
    CHECK( pool.getSize() == 0 );

    mpool::Id64 id{ pool.alloc( "zero" ) };

    CHECK( pool.isIdValid( id ) );
    CHECK( pool.getCapacity() == 2 );
    CHECK( pool.getSize() == 1 );
    CHECK( *pool.getPtr( id ) == std::string( "zero" ) );

    auto id1( pool.alloc( "one" ) );

    CHECK( pool.isIdValid( id1 ) );
    CHECK( pool.getCapacity() == 2 );
    CHECK( pool.getSize() == 2 );
    CHECK( pool[ id1 ] == "one" );

    id = pool.alloc( "two" );

    CHECK( pool.isIdValid( id ) );
    CHECK( pool.getCapacity() == 4 );
    CHECK( pool.getSize() == 3 );
    CHECK( pool[ id ] == "two" );

    auto id3 = pool.alloc( "three" );

    CHECK( pool.isIdValid( id3 ) );
    CHECK( pool.getCapacity() == 4 );
    CHECK( pool.getSize() == 4 );
    CHECK( pool[ id3 ] == "three" );

    id = pool.alloc( "four" );

    CHECK( pool.isIdValid( id ) );
    CHECK( pool.getCapacity() == 6 );
    CHECK( pool.getSize() == 5 );
    CHECK( pool[ id ] == "four" );

    pool.free( id1 );
    pool.free( id3 );

    id = pool.alloc( "nova" );
    CHECK( pool.isIdValid( id ) );
    CHECK( id.index() == 1 );

    id = pool.alloc( "super nova" );
    CHECK( id.index() == 3 );
}


TEST_CASE( "Signal correctness", "[signal]" )
{
    struct Observer
    {
        void onSignal1( int i )
        {
            std::cout << "onSignal1 executes; parameters:: i: " << i << std::endl;
            sig1Sum += i;
        }

        int sig1Sum{ 0 };
    };

    util::Signal< int > sig1{};
    Observer oer{};

    auto connection( sig1.connect( std::bind( &Observer::onSignal1, &oer, std::placeholders::_1 ) ) );

    sig1( 8 );
    sig1( 22 );

    CHECK( oer.sig1Sum == 30 );

    sig1.disconnect( connection );
    sig1( 5 );

    CHECK( oer.sig1Sum == 30 );
}

TEST_CASE( "ECS correctness", "[tktk-ecs]" )
{
    class Comp1
    : public ecs::Comp< Comp1 >
    {
    public:
        Comp1( const mpool::Id64& entityId )
        : BaseTypeT{ entityId }
        {
        }

    public:
        std::string name{ "noname" };
    };

    class Proc1
    : public ecs::Proc< Comp1 >
    {
    public:
        Proc1(ecs::System* systemPtr)
        : ecs::Proc< Comp1 >( systemPtr )
        {
        }

        virtual void setup() override
        {
            mSystemPtr->updateSignal.connect( std::bind( &Proc1::onUpdate, this, std::placeholders::_1 ) );
        }

        virtual void onUpdate( float deltaTime )
        {
            std::cout << "Proc1::onUpdate: " << deltaTime << std::endl;
//             for ( int i{ 0 }; i < mPool.getSize(); ++i )
//             {
//                 std::cout << mPool.getPtr( i )->name << std::endl;
//             }
            mPool.forEach(
                [&] ( Comp1& c )
                {
                    std::cout << c.name << std::endl;
                }
            );
        }

    };

    class Comp2
    : public ecs::Comp< Comp2 >
    {
    public:
        Comp2( const mpool::Id64& entityId )
        : BaseTypeT{ entityId }
        {
        }

    public:
        std::size_t number{ 0 };
    };

    class Proc2
    : public ecs::Proc< Comp2 >
    {
    public:
        Proc2(ecs::System* systemPtr)
        : ecs::Proc< Comp2 >( systemPtr )
        {
        }

        virtual void setup() override
        {
            mSystemPtr->updateSignal.connect( std::bind( &Proc2::onUpdate, this, std::placeholders::_1 ) );
        }

        virtual void onUpdate( float deltaTime )
        {
            std::cout << "Proc2::onUpdate: " << deltaTime << std::endl;
//             for ( int i{ 0 }; i < mPool.getSize(); ++i )
//             {
//                 std::cout << mPool.getPtr( i )->number << std::endl;
//             }
            mPool.forEach(
                [&] ( Comp2& c )
                {
                    std::cout << c.number << std::endl;
                }
            );
        }
    };


    ecs::System ecs;

    ecs.registerProc< Proc1 >();
    ecs.registerProc< Proc2 >();

    ecs.setup();

    Proc1* proc1{ ecs.getProc< Proc1 >() };
    auto proc2( ecs.getProc< Proc2 >() );

    ecs::Entity::Handle e1{ ecs.addEntity() };

    Comp1::Handle c1( proc1->addComp( e1.getId() ) );
    auto c2( proc2->addComp( e1.getId() ) );

    float timeStep{ 0.05f };
    float time{ 0.0f };
    while ( time < 1.0f )
    {
        if ( time > 0.5f && time < 0.6f )
        {
            c1->name = "somename";
            c2->number = 8;
        }
        ecs.update( timeStep );
        time += timeStep;
    }
    std::cout << "LOOP FINISHED" << std::endl;
}
