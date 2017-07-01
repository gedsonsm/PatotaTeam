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
				if (self->supplyUsed() >= self->supplyTotal()) //verifica se está faltando população
				{
					//tem recusos para treinar um overlord?
					if (self->incompleteUnitCount(overlord) == 0 && util->qtdLarva > 0 && self->minerals() >= overlord.mineralPrice())
					{
						hq->train(overlord);
					}
				}
				//cria um drone se o num de drones está a baixo do min ou se ja tem o max de zerling ou se um drone foi usado para criar uma pool
				else if ((util->qtdDrone < MAX_DRONES || (util->qtdZergling > MAX_ZERGLING && util->qtdDrone < MAX_DRONES) || util->substituiDrone) && util->qtdLarva > 0 && self->minerals() >= drone.mineralPrice())
				{
					hq->train(drone);
					util->substituiDrone = false; // o drone já foi substituido
				}
				else if (util->temPool)// verifica se ja tem uma pool
				{
					if (util->qtdLarva > 0 && self->minerals() >= zergling.mineralPrice()) // se tem recursos sufucientes para treinar zerglings
					{
						hq->train(zergling);
					}
				}
				else if (self->incompleteUnitCount(UnitTypes::Zerg_Spawning_Pool) == 0) //se não tem uma pool
				{
					//verifica se tem recursos suficientes para construir uma pool
					if (self->minerals() >= (spawningPool.mineralPrice() + drone.mineralPrice()) && !util->construirPool && !util->construindoPool)
					{
						util->construirPool = true; // dá a ordem para contruir uma pool(quem vai construir é um drone)
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