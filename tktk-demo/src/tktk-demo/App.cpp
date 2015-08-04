/**
    tktk

    tktk - Copyright (C) 2015 Stanislav Demyanovich <stan@angrybubo.com>

    This software is provided 'as-is', without any express or
    implied warranty. In no event will the authors be held
    liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute
    it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented;
       you must not claim that you wrote the original software.
       If you use this software in a product, an acknowledgment
       in the product documentation would be appreciated but
       is not required.

    2. Altered source versions must be plainly marked as such,
       and must not be misrepresented as being the original software.

    3. This notice may not be removed or altered from any
       source distribution.
*/

// #include <tktk/log/loggingDefs.h>

#include "App.hpp"
#include <tktk/phase/PhaseManager.hpp>
#include "ThemeSettingsWindow.hpp"
#include "SfguiWidgetSetWindow.hpp"
#include "EntryExampleWindow.hpp"
#include "RangeExampleWindow.hpp"

using namespace tktk;

namespace testapp
{

App::App()
: mAssetsPath( "./assets/testapp/" )
{
    mPhaseManager = std::make_shared<phase::PhaseManager>();
}

App::~App()
{
}

void App::run()
{
  mAppWindow.create( sf::VideoMode( 2000, 1500 ), "Triko testapp" );

  onReloadTheme();

  auto themeWindow = std::make_shared<ThemeSettingsWindow>();
  themeWindow->onCreate();
  themeWindow->SetPosition( sf::Vector2f( 50, 50 ) );
  themeWindow->setOnReloadTheme( std::bind( &App::onReloadTheme, this ));

  auto widgetSetWindow = std::make_shared<SfguiWidgetSetWindow>();
  widgetSetWindow->onCreate();
  widgetSetWindow->SetPosition( sf::Vector2f( 50, 500 ) );

  auto entryExample = std::make_shared<EntryExampleWindow>();
  entryExample->onCreate();
  entryExample->SetTitle( L"Entry example" );
  entryExample->SetPosition( sf::Vector2f( 350, 50 ) );

  auto rangeExample = std::make_shared<RangeExampleWindow>();
  rangeExample->onCreate();
  rangeExample->SetTitle( L"Range example" );
  rangeExample->SetPosition( sf::Vector2f( 700, 50 ) );

  mGuiDesktop.Add( themeWindow );
  mGuiDesktop.Add( widgetSetWindow );
  mGuiDesktop.Add( entryExample );
  mGuiDesktop.Add( rangeExample );


  mAppWindow.resetGLStates();

  sf::Event event;
  sf::Clock clock;

  while ( mAppWindow.isOpen() )
  {
    while ( mAppWindow.pollEvent( event ) )
    {
      mGuiDesktop.HandleEvent( event );

      if (event.type == sf::Event::Closed)
      {
        mAppWindow.close();
      }
    }

    mGuiDesktop.Update( clock.restart().asSeconds() );
    mPhaseManager->update();

    mAppWindow.clear();
    mGui.Display( mAppWindow );
    mAppWindow.display();
  }
}

void App::onReloadTheme()
{
  std::string tmp = mAssetsPath + "sfgui-tktk.theme";
  // mLogger.debug( "Loading theme: " + tmp );
  mGuiDesktop.LoadThemeFromFile( tmp );
}

} //namespace testapp
