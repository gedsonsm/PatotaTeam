#include "ZergDrone.h"


ZergDrone::ZergDrone(Unit unit)
{
	CreateThread(NULL, 0, ZergDrone::run, (LPVOID)unit, 0, NULL);
}


ZergDrone::~ZergDrone()
{
}

DWORD WINAPI ZergDrone::run(LPVOID param){
	BWAPI::Unit unit = static_cast<BWAPI::Unit>(param);
	DWORD dwWaitResult;
	Unit target = nullptr;
	bool returningCargo = true;
	bool isGathering = false;

	Unit lastUnitTarget = nullptr;
	Unit unitTarget;

	while (true){

		dwWaitResult = WaitForSingleObject(
			Utils->ghMutex,    // handle to mutex
			100);  // time-out interval

		//// If end game, or if it exists (remember to always check)
		if (Utils->GameOver || unit == NULL || !unit->exists())  {
			ReleaseMutex(Utils->ghMutex);
			return 0; // end thread
		} // end thread
		// You can check tons of others things like isStuck, isLockedDown, constructing
		if (!unit->isCompleted() || !unit->isCompleted()){ // You can create it on the onUnitComplete too!
			ReleaseMutex(Utils->ghMutex);
			Sleep(500);
			continue;
		}

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) //RAII
		{
			if (Utils->construirPool && !Utils->construindoPool) {
				TilePosition targetBuildLocation = Broodwar->getBuildLocation(UnitTypes::Zerg_Spawning_Pool, unit->getTilePosition());
				if (targetBuildLocation)
				{
					unit->build(UnitTypes::Zerg_Spawning_Pool, targetBuildLocation);
					Utils->construirPool = false;
					Utils->construindoPool = true;
					Utils->subistituiDronePool = true;
				}
				ReleaseMutex(Utils->ghMutex);
				return 0;
			}

			unitTarget = Utils->getAlvoDefesa(unit);
			if (unitTarget != nullptr) {
				if (unit->isCarryingMinerals()) {
					returningCargo = false;
					isGathering = false;
				}
				else {
					returningCargo = true;
					isGathering = false;
				}
				if (unitTarget != lastUnitTarget) {
					unit->attack(unitTarget);
					lastUnitTarget = unitTarget;
				}
			} 
			else if (unit->isCarryingGas() || unit->isCarryingMinerals())
			{
				isGathering = false;
				
				if (!returningCargo) {
					unit->returnCargo();

					Mineral->leaveQueue(target);
					target = nullptr;
					returningCargo = true;
				}
			}
			else if (!unit->getPowerUp())
			{
				if (returningCargo) {
					target = Mineral->getMineralField(unit);
					returningCargo = false;
				}
				if (target != nullptr) {
					if (target->exists())	{
						if (unit->getOrderTarget() == NULL) {
							if (!isGathering) {
								isGathering = true;
								if (!unit->gather(target)) {
									//handle error
								}
							}
						}
						else {
							if (!isGathering || (unit->getOrderTarget() != target && unit->getOrderTarget()->getType().isMineralField())) {
								isGathering = true;
								if (!unit->gather(target)) {
									//handle error
								}
							}
						}

					}
					else {
						returningCargo = true;
					}
				}
				else {
					returningCargo = true;
				}
			} // closure: has no powerup
			
			if (!ReleaseMutex(Utils->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(30); // Some agents can sleep more than others. 
	}
}
