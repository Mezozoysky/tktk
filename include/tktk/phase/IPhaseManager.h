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

#ifndef TKTK_PHASE_I_PHASE_MANAGER_H
#define TKTK_PHASE_I_PHASE_MANAGER_H

#include <tktk/phase/IPhase.h>
#include <memory>
#include <string>

namespace tktk
{
namespace phase
{

class IPhaseManager;
typedef std::shared_ptr<IPhaseManager> IPhaseManagerPtr;

class IPhaseManager
{
public:

  virtual ~IPhaseManager()
  {
  }

  virtual void registerPhase( IPhasePtr phase ) = 0; // Register the new phase with the manager.

  virtual void switchPhase( const std::string& phaseName ) = 0; // Replace the top phase with another registered one specified by name.

  virtual void pushPhase( const std::string& phaseName ) = 0; // Pause the top phase, make another registered one the top and activate it.
  virtual void popPhase() = 0; // Deactivate the top phase, pop it, resume top phase.

  virtual void clearPhases() = 0; //Deactivate and unregister all the phases.

  virtual void update() = 0; //Update current phase.

  virtual const std::string& getStartPhase() const = 0; // Return the name of the starting phase.
  virtual void setStartPhase( const std::string& phaseName ) = 0; // Set the name of starting phase.
};

} //namespace phase
} //namespace tktk

#endif //TKTK_PHASE_I_PHASE_MANAGER_H
