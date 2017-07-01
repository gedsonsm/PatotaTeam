#pragma once
#include <BWAPI.h>
#include "Hatchery.h"
#include "SpawningPool.h"

#include "ZergDrone.h"
#include "Zergling.h"
#include "Overlord.h"
#include "GerenciadorAtaque.h"
#include "Utilitarios.h"

// Remember not to use "Broodwar" in any global class constructor!

class PatotaTeamBot : public BWAPI::AIModule
{
public:
	// Virtual functions for callbacks, leave these as they are.
	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();
	virtual void onUnitCreate(BWAPI::Unit unit);
	virtual void onUnitDestroy(BWAPI::Unit unit);
	virtual void onUnitMorph(BWAPI::Unit unit);
	// Everything below this line is safe to modify.

};
