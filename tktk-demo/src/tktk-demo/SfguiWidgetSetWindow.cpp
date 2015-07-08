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

#include "SfguiWidgetSetWindow.h"
#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/Adjustment.hpp>

namespace testapp
{

SfguiWidgetSetWindow::SfguiWidgetSetWindow()
: GuiWindow()
{
}

SfguiWidgetSetWindow::~SfguiWidgetSetWindow()
{
}

void SfguiWidgetSetWindow::onCreate()
{
  SetTitle( L"SFGUI Widget Set" );

  auto topBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );

  {
    auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );

    auto button = sfg::Button::Create();
    button->SetLabel( "Button" );
    auto label = sfg::Label::Create();
    label->SetText( "label with some text" );
    auto entry = sfg::Entry::Create();
    entry->SetRequisition( sf::Vector2f( 150.0f, 0.0f ) );
    entry->SetText( "some text" );

    box->Pack( button );
    box->Pack( label );
    box->Pack( entry );

    box->SetSpacing( 8.0f );

    topBox->Pack( box );
  }

  {
    auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );
    auto label = sfg::Label::Create();
    label->SetText( "20" );
    auto scale = sfg::Scale::Create( sfg::Scale::Orientation::HORIZONTAL );
    auto scrollbar = sfg::Scrollbar::Create( sfg::Scrollbar::Orientation::VERTICAL );
    auto adjustment = scrollbar->GetAdjustment();
    scale->SetAdjustment( adjustment );
    adjustment->SetLower( 20.0f );
    adjustment->SetUpper( 100.0f );
    adjustment->SetMinorStep( 3.0f );
    adjustment->SetMajorStep( 12.0f );
    adjustment->SetPageSize( 20.0f );
    //adjustment signal
    scale->SetRequisition( sf::Vector2f( 80.0f, 20.0f ) );
    scrollbar->SetRequisition( sf::Vector2f( 0.0f, 80.0f ) );

    auto scalebox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );
    scalebox->Pack( scale, false, false );
    box->Pack( scalebox );
    box->Pack( scrollbar, false, false );
    box->Pack( label );
    box->SetSpacing( 8.0f );

    topBox->Pack( box );
  }

  Add( topBox );
  topBox->SetSpacing( 8.0f );
}

} //namespace testapp
