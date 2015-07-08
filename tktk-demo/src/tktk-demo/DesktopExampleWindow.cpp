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

#include "DesktopExampleWindow.h"
#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Context.hpp>
#include <sstream>

namespace testapp
{

DesktopExampleWindow::DesktopExampleWindow()
: GuiWindow()
, mCount( 0 )
{
}

DesktopExampleWindow::~DesktopExampleWindow()
{
}

void DesktopExampleWindow::onCreate()
{
  auto introLabel = sfg::Label::Create( "Click on \"Create window\" to create any number of new windows." );
  auto createWindowButton = sfg::Button::Create( "Create window" );
  createWindowButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &DesktopExampleWindow::onCreateWindowClick, this) );
  createWindowButton->SetId( "create_window_btn" );

  auto mainBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
  mainBox->Pack( introLabel, false );
  mainBox->Pack( createWindowButton, false );

  Add( mainBox );
}

void DesktopExampleWindow::onCreateWindowClick()
{
  ++mCount;

  auto window = sfg::Window::Create();

  std::stringstream sstr;
  sstr << "A new window #"<< mCount;
  window->SetTitle( sstr.str() );

  auto destroyButton = sfg::Button::Create( "Destroy" );
  auto frontButton = sfg::Button::Create( "Desktop example window to front" );

  auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f );
  box->Pack( sfg::Label::Create( "This is a newly created window, from runtime, interactively." ), false );
  box->Pack( sfg::Label::Create( "You can move me around, try it!" ), false );
  box->Pack( sfg::Label::Create( "Or click the button below to destroy me. :-(" ), false );
  box->Pack( destroyButton, false );
  box->Pack( frontButton, false );

  window->Add( box );
//  m_desktop.Add( window );

  // Signals.
  destroyButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &DesktopExampleWindow::onDestroyWindowClick, this ) );
  frontButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( std::bind( &DesktopExampleWindow::onFrontClick, this ) );
}

void DesktopExampleWindow::onDestroyWindowClick() {

  auto widget = sfg::Context::Get().GetActiveWidget();

  while( widget->GetName() != "Window" ) {
    widget = widget->GetParent();
  }

//  this->
  // Remove window from desktop.
//  m_desktop.Remove( widget );
}

void DesktopExampleWindow::onFrontClick() {
//  m_desktop.BringToFront( m_window );
}


} //namespace testapp
