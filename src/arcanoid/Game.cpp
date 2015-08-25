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
#include <iostream>

using namespace tktk;

Game::Game()
: mIsRunning{ false }
{
    mWindow.create( { 640, 480 }, "tktk Arcanoid" );
}

Game::~Game()
{
}

void Game::run()
{
    auto transformProc( mECS.addProcessor<TransformProcessor>() );
    auto e1( mECS.getEntityManager()->createEntity() );
    auto e2( mECS.getEntityManager()->createEntity() );
    auto e3( mECS.getEntityManager()->createEntity() );

//     auto tf1( transformProc->addComponent( e1 ) );
//     tf1->position() = sf::Vector2f( 100.0f, 75.0f );

    mWindow.setFramerateLimit(60);

    sf::Clock clock;
    mIsRunning = true;
    while ( mIsRunning && mWindow.isOpen() )
    {
        sf::Event event;
        while ( mWindow.pollEvent( event ) )
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    mIsRunning = false;
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    std::cout << "\tKey is pressed!" << std::endl;
                }
                default:
                {
                    break;
                }
            }
        }

        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Escape ) )
        {
            mIsRunning = false;
        }

        mWindow.clear();
        float secondsElapsed{ clock.restart().asSeconds() };
        mECS.update( secondsElapsed );
        mWindow.display();

//         std::cout << "FPS: " << std::to_string( 1.0f / secondsElapsed ) << std::endl;
        std::cout.flush();
    }

    mWindow.close();
}
