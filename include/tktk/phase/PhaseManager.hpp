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

#ifndef TKTK_PHASE_PHASE_MANAGER_HPP
#define TKTK_PHASE_PHASE_MANAGER_HPP

#include <tktk/phase/IPhase.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <stack>
#include <type_traits>
#include <cassert>

// #include <tktk/log/loggingDefs.h>


namespace tktk
{
namespace phase
{

class PhaseManager;
typedef std::shared_ptr<PhaseManager> PhaseManagerPtr;

class PhaseManager
{
public:
  using PhaseMapT = std::unordered_map<std::string, IPhasePtr>;

  PhaseManager();
  virtual ~PhaseManager();

  template< typename T, typename... TArgs >
  T& createPhase( TArgs&&... args ) // Creates the new phase with the manager.
  {
      static_assert(
          std::is_base_of< IPhase, T >::value
          , "Incompatible type for phase object instantiation. Must be derived from IPhase."
      );

      T* phase{ new T( std::forward< TArgs >( args )._Bool... ) };
      std::unique_ptr< IPhase > phaseUPtr{ phase };

      auto pair( mPhases.emplace( phase->getName(), std::move( phaseUPtr ) ) );
      assert( pair.second == true );

      return ( *phase );
  }

  virtual void switchPhase( const std::string& phaseName ); // Replace the top phase with another registered one specified by name.

  virtual void pushPhase( const std::string& phaseName ); // Pause the top phase, make another registered one the top and activate it.
  virtual void popPhase(); // Deactivate the top phase, pop it, resume top phase.

  virtual void clearPhases(); //Deactivate and unregister all the phases.

  virtual void update(); //Update current phase.

  virtual const std::string& getStartPhase() const; // Return the name of the starting phase.
  virtual void setStartPhase( const std::string& phaseName ); // Set the name of starting phase.

protected:
  // log::Logger mLogger;
  std::string mStartPhase; //Phase from wich game will be started
  PhaseMapT mPhases; //List of all phases in game
  std::stack<IPhasePtr> mStack; //Stack for 'state machine'
  unsigned int mPhasesCount;
};

} //namespace phase
} //namespace tktk

#endif //TKTK_PHASE_PHASE_MANAGER_HPP
