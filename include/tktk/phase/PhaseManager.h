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

#ifndef TKTK_PHASE_PHASE_MANAGER_H
#define TKTK_PHASE_PHASE_MANAGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <stack>

// #include <tktk/log/loggingDefs.h>
#include <tktk/phase/IPhaseManager.h>

namespace tktk
{
namespace phase
{

class PhaseManager;
typedef std::shared_ptr<IPhaseManager> PhaseManagerPtr;

class PhaseManager
: public IPhaseManager
{
public:
  using PhaseMapT = std::unordered_map<std::string, IPhasePtr>;

  PhaseManager();
  virtual ~PhaseManager();

  virtual void registerPhase( IPhasePtr phase ) override; // Register the new phase with the manager.

  virtual void switchPhase( const std::string& phaseName ) override; // Replace the top phase with another registered one specified by name.

  virtual void pushPhase( const std::string& phaseName ) override; // Pause the top phase, make another registered one the top and activate it.
  virtual void popPhase() override; // Deactivate the top phase, pop it, resume top phase.

  virtual void clearPhases() override; //Deactivate and unregister all the phases.

  virtual void update() override; //Update current phase.

  virtual const std::string& getStartPhase() const override; // Return the name of the starting phase.
  virtual void setStartPhase( const std::string& phaseName ) override; // Set the name of starting phase.

protected:
  // log::Logger mLogger;
  std::string mStartPhase; //Phase from wich game will be started
  PhaseMapT mPhases; //List of all phases in game
  std::stack<IPhasePtr> mStack; //Stack for 'state machine'
  unsigned int mPhasesCount;
};

} //namespace phase
} //namespace tktk

#endif //TKTK_PHASE_PHASE_MANAGER_H
