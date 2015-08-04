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

#include <tktk/phase/PhaseManager.hpp>

namespace tktk
{
namespace phase
{

PhaseManager::PhaseManager()
: mPhasesCount( 0 )
{
  // BOOST_LOG_TRIVIAL(trace) << "PhaseManager ctor()";
  //
  // mLogger.info( "Cool info message" );
  // log::simple::log( "Simple message" );
}

PhaseManager::~PhaseManager()
{
  // BOOST_LOG_TRIVIAL(trace) << "PhaseManager dtor()";
}

/*
void PhaseManager::registerPhase( IPhasePtr phase )
{
//  BOOST_LOG_TRIVIAL(trace) << "PhaseManager registerPhase()";
  // const auto cit = mPhases.find( phase );
  // if ( cit == mPhases.end() ) // not found
  // {
  //   mPhases[ phase->getName() ] = phase;
  // }
}
*/

void PhaseManager::switchPhase( const std::string& phaseName )
{
//  BOOST_LOG_TRIVIAL(trace) << "PhaseManager switchPhase()";
}

void PhaseManager::pushPhase( const std::string& phaseName )
{

}

void PhaseManager::popPhase()
{

}

void PhaseManager::clearPhases()
{

}

void PhaseManager::update()
{
  // if ( !mStask.empty() )
  // {
  //   mStack.top()->update();
  // }
  // BOOST_LOG_SCOPED_THREAD_ATTR( "Tag", boost::log::attributes::constant<std::string>( "[PhaseManager::update]" ) );
  //
  // BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
  // BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
  // BOOST_LOG_TRIVIAL(info) << "An informational severity message";
  // BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
  // BOOST_LOG_TRIVIAL(error) << "An error severity message";
  // BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
  //
  //
  // BOOST_LOG( mLogger ) << "Log message from update()";
}

const std::string& PhaseManager::getStartPhase() const
{
  return mStartPhase;
}

void PhaseManager::setStartPhase( const std::string& phaseName )
{
  mStartPhase = phaseName;
}

} //namespace phase
} //namespace tktk
