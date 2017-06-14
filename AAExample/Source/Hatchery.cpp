#include "Hatchery.h"

Hatchery::Hatchery(Unit unit)
{
	CreateThread(NULL, 0, Hatchery::run, (LPVOID)unit, 0, NULL);
}


Hatchery::~Hatchery()
{
}

DWORD WINAPI Hatchery::run(LPVOID param){

	BWAPI::Unit hq = static_cast<BWAPI::Unit>(param);
	DWORD dwWaitResult;
	Player self = BWAPI::Broodwar->self();
	UnitType overlord = hq->getType().getRace().getSupplyProvider();
	UnitType drone = hq->getType().getRace().getWorker();
	UnitType zergling = BWAPI::UnitTypes::Zerg_Zergling;
	UnitType spawningPool = BWAPI::UnitTypes::Zerg_Spawning_Pool;

	while (true){

		dwWaitResult = WaitForSingleObject(
			Utils->ghMutex,    // handle to mutex
			100);  // time-out interval

		if (Utils->GameOver || hq == NULL || !hq->exists()) {
			ReleaseMutex(Utils->ghMutex);
			return 0; // end thread
		}
		// Some things are commom between units, so you can apply a little of OO here.

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED)
		{
			if (hq->isIdle()){
				if (self->supplyUsed() >= self->supplyTotal()){
					if (self->incompleteUnitCount(overlord) == 0 && Utils->qtdLarva > 0 && self->minerals() >= overlord.mineralPrice()) {
						hq->train(overlord);
					}
				}
				else if ((Utils->qtdDrone < MIN_DRONES || (Utils->qtdZergling > MAX_ZERGLING && Utils->qtdDrone < MAX_DRONES) || Utils->subistituiDronePool) && Utils->qtdLarva > 0 && self->minerals() >= drone.mineralPrice()) {
					hq->train(drone);
					Utils->subistituiDronePool = false;
				} else if (Utils->temPool) {
					if (Utils->qtdLarva > 0 && self->minerals() >= zergling.mineralPrice()) {
						hq->train(zergling);
					}
				}
				else if (self->incompleteUnitCount(UnitTypes::Zerg_Spawning_Pool) == 0) {
					if (self->minerals() >= (spawningPool.mineralPrice() + drone.mineralPrice()) && !Utils->construirPool && !Utils->construindoPool) {
						Utils->construirPool = true;
					}
				}
			}

			// Release ownership of the mutex object
			if (!ReleaseMutex(Utils->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(20);
	}
}
