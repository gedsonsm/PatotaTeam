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
			util->ghMutex,    // handle to mutex
			100);  // time-out interval

		if (util->GameOver || hq == NULL || !hq->exists()) {
			ReleaseMutex(util->ghMutex);
			return 0; // end thread
		}
		// Some things are commom between units, so you can apply a little of OO here.

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED)
		{
			if (hq->isIdle())
			{
				if (self->supplyUsed() >= self->supplyTotal() && !util->treinandoOverlord)
				{
					if (self->incompleteUnitCount(overlord) == 0 && util->qtdLarva > 0 && self->minerals() >= overlord.mineralPrice()) 
					{
						hq->train(overlord);
						util->treinandoOverlord = true;
					}
				}
				else if (((util->qtdDrone < MIN_DRONES || (util->qtdZergling > MAX_ZERGLING && util->qtdDrone < MAX_DRONES)) && util->qtdLarva > 0 && self->minerals() >= drone.mineralPrice()) || util->subistituiDronePool)
				{
					hq->train(drone);
					util->subistituiDronePool = false;
				} 
				else if (util->temPool) 
				{
					if (util->qtdLarva > 0 && self->minerals() >= zergling.mineralPrice()) 
					{
						hq->train(zergling);
					}
				}
				else if (self->incompleteUnitCount(UnitTypes::Zerg_Spawning_Pool) == 0) 
				{
					if (self->minerals() >= (spawningPool.mineralPrice() + drone.mineralPrice()) && !util->construirPool && !util->construindoPool)
					{
						util->construirPool = true;
					}
				}
			}

			// Release ownership of the mutex object
			if (!ReleaseMutex(util->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(20);
	}
}
