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
#include <tktk/util/Signal.hpp>
#include <tktk/ecs/System.hpp>
//#include <tktk/ecs/Component.hpp>
//#include <tktk/ecs/Processor.hpp>
#include <string>
#include <iostream>

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

    pool.destroyElement( 1 );
    pool.destroyElement( 3 );

    index = pool.createElement( "nova" );
    CHECK( index == 1 );

    index = pool.createElement( "super nova" );
    CHECK( index == 3 );
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
    class Comp
    : public ecs::Component< Comp >
    {
    public:
        Comp( const ecs::EntityHandle& ownerHandle )
        : BasalType{ ownerHandle }
        {
        }

    public:
        std::string name{ "noname" };
    };

    class Proc
    : public ecs::Processor< Proc, Comp >
    {
    public:
        virtual void onUpdate( float deltaTime ) override
        {
            std::cout << "Proc::onUpdate: " << deltaTime << std::endl;
            for ( int i{ 0 }; i < mComponents.getSize(); ++i )
            {
                std::cout << mComponents[ i ].name << std::endl;
            }
        }

    };

    class Comp2
    : public ecs::Component< Comp2 >
    {
    public:
        Comp2( const ecs::EntityHandle& ownerHandle )
        : BasalType{ ownerHandle }
        {
        }

    public:
        std::size_t number{ 0 };
    };

    class Proc2
    : public ecs::Processor< Proc2, Comp2 >
    {
    public:
        virtual void onUpdate( float deltaTime )
        {
            std::cout << "Proc2::onUpdate: " << deltaTime << std::endl;
            for ( int i{ 0 }; i < mComponents.getSize(); ++i )
            {
                std::cout << mComponents[ i ].number << std::endl;
            }
        }

    };


    ecs::System<> ecs;

    ecs.registerProcessor< Proc >();
    ecs.registerProcessor< Proc2 >();

    ecs.setup();

    std::shared_ptr< Proc > proc{ ecs.getProcessor< Proc >() };
    auto proc2( ecs.getProcessor< Proc2 >() );

    ecs::EntityHandle e1{ ecs.entityManager.createEntity() };

    ecs::ComponentHandle< Proc > c1{ proc->addComponent( e1 ) };
    auto c2( proc2->addComponent( e1 ) );

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
}
