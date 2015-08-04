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

#include "EntryExampleWindow.hpp"
#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>

namespace testapp
{

EntryExampleWindow::EntryExampleWindow()
: GuiWindow()
{
}

EntryExampleWindow::~EntryExampleWindow()
{
}

void EntryExampleWindow::onCreate()
{
  auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );

  auto button = sfg::Button::Create();
  button->SetLabel( "Set" );
  button->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &EntryExampleWindow::onSetButtonClick, this ) );
  mLabel = sfg::Label::Create();
  mLabel->SetText( "no text yet" );
  mEntry = sfg::Entry::Create();
  mEntry->SetRequisition( sf::Vector2f( 80.0f, 0.0f ) );

  box->Pack( mEntry );
  box->Pack( button );
  box->Pack( mLabel );

  box->SetSpacing( 8.0f );

  Add( box );
}

void EntryExampleWindow::onSetButtonClick()
{
    mLabel->SetText( mEntry->GetText() );
}

} //namespace testapp
