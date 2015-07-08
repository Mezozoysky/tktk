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

#include "ThemeSettingsWindow.h"
#include <SFGUI/Label.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>

namespace testapp
{

ThemeSettingsWindow::ThemeSettingsWindow()
: GuiWindow()
, mOnReloadTheme( nullptr )
{
}

ThemeSettingsWindow::~ThemeSettingsWindow()
{
}

void ThemeSettingsWindow::onCreate()
{
  SetTitle( L"Theme Settings" );

  auto label = sfg::Label::Create( "SFGUI-TKTK Theme" );
  auto button = sfg::Button::Create( "Reload theme" );
  button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &ThemeSettingsWindow::onReloadButtonClick, this ) );

  auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
  box->Pack( label );
  box->Pack( button, false );

  Add( box );
}

void ThemeSettingsWindow::setOnReloadTheme( std::function<void()> handler )
{
  mOnReloadTheme = handler;
}

void ThemeSettingsWindow::onReloadButtonClick()
{
  if ( mOnReloadTheme == nullptr )
  {
    return;
  }
  mOnReloadTheme();
}

} //namespace testapp
