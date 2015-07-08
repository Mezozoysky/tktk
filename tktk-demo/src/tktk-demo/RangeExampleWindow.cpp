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

#include "RangeExampleWindow.h"
#include <SFGUI/Box.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <sstream>

namespace testapp
{

RangeExampleWindow::RangeExampleWindow()
: GuiWindow()
{
}

RangeExampleWindow::~RangeExampleWindow()
{
}

void RangeExampleWindow::onCreate()
{
  auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );

  mLabel = sfg::Label::Create();
  mLabel->SetText( "20" );
  auto scale = sfg::Scale::Create( sfg::Scale::Orientation::HORIZONTAL );
  auto scrollbar = sfg::Scrollbar::Create( sfg::Scrollbar::Orientation::VERTICAL );
  mAdjustment = scrollbar->GetAdjustment();
  scale->SetAdjustment( mAdjustment );
  mAdjustment->SetLower( 20.0f );
  mAdjustment->SetUpper( 100.0f );
  mAdjustment->SetMinorStep( 3.0f );
  mAdjustment->SetMajorStep( 12.0f );
  mAdjustment->SetPageSize( 20.0f );
  mAdjustment->GetSignal( sfg::Adjustment::OnChange ).Connect( std::bind( &RangeExampleWindow::onAdjustmentChanged, this ) );
  //adjustment signal
  scale->SetRequisition( sf::Vector2f( 80.0f, 20.0f ) );
  scrollbar->SetRequisition( sf::Vector2f( 0.0f, 80.0f ) );

  auto scalebox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL );
  scalebox->Pack( scale, false, false );
  box->Pack( scalebox );
  box->Pack( scrollbar );
  box->Pack( mLabel );
  box->SetSpacing( 8.0f );

  Add( box );
}

void RangeExampleWindow::onAdjustmentChanged()
{
  std::stringstream sstr;
  sstr << mAdjustment->GetValue();
    mLabel->SetText( sstr.str() );
}

} //namespace testapp
