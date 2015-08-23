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
    // Limit the framerate to 60 frames per second (this step is optional)
    mWindow.setFramerateLimit(60);

    mIsRunning = true;
    while ( mIsRunning && mWindow.isOpen() )
    {
        sf::Event event;
        while ( mWindow.pollEvent( event ) )
        {
            // Request for closing the window
            if ( event.type == sf::Event::Closed )
            {
                mIsRunning = false;
            }
        }

        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Escape ) )
        {
            mIsRunning = false;
        }

        mWindow.clear();
        mECS.update( 0.001f );
        mWindow.display();
    }

    mWindow.close();
}
