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
	Unit algo = nullptr;
	bool devolverCarga = true;
	bool estaColetanto = false;

	Unit lastUnitTarget = nullptr;
	Unit unitTarget;

	while (true){

		dwWaitResult = WaitForSingleObject(
			util->ghMutex,    // handle to mutex
			100);  // time-out interval

		//// If end game, or if it exists (remember to always check)
		if (util->GameOver || unit == NULL || !unit->exists())  {
			ReleaseMutex(util->ghMutex);
			return 0; // end thread
		} // end thread
		// You can check tons of others things like isStuck, isLockedDown, constructing
		if (!unit->isCompleted() || !unit->isCompleted()){ // You can create it on the onUnitComplete too!
			ReleaseMutex(util->ghMutex);
			Sleep(500);
			continue;
		}

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) //RAII
		{
			if (util->construirPool && !util->construindoPool)  // verifica se existe uma ordem para construir uma pool e uma já não está sendo construida
			{
				TilePosition localConstrucao = Broodwar->getBuildLocation(UnitTypes::Zerg_Spawning_Pool, unit->getTilePosition());
				if (localConstrucao) //se tem um lugar pra costruie
				{
					unit->build(UnitTypes::Zerg_Spawning_Pool, localConstrucao);
					util->construirPool = false;
					util->construindoPool = true;
					util->substituiDrone = true;
				}
				ReleaseMutex(util->ghMutex); //libera o mutex, pois o drone vai deixar de existir
				return 0;
			}

			unitTarget = util->getAlvoDefesa(unit);
			if (unitTarget != nullptr)  // se existe um algo que está atacando
			{
				if (unit->isCarryingMinerals()) // se estiver carregando material, vá devolver
				{
					devolverCarga = false;
					estaColetanto = false;
				}
				else 
				{
					devolverCarga = true;
					estaColetanto = false;
				}
				if (unitTarget != lastUnitTarget) // ataca ate matar ou morrer
				{
					unit->attack(unitTarget);
					lastUnitTarget = unitTarget;
				}
			} 
			else if (unit->isCarryingGas() || unit->isCarryingMinerals()) //se está carregando algo
			{
				estaColetanto = false; //para de coletar
				
				if (!devolverCarga)  // devolve a carga
				{
					unit->returnCargo();

					Mineral->leaveQueue(algo);
					algo = nullptr;
					devolverCarga = true;
				}
			}
			else if (!unit->getPowerUp()) //não sei porque, mas tive dificuldade de tirar esse if, deve ser algo da api
			{
				if (devolverCarga) //se já devolveu
				{
					algo = Mineral->getMineralField(unit); //pega um campo de minerio mais proximo
					devolverCarga = false;
				}
				if (algo != nullptr)  //se tem campo para coletar
				{
					if (algo->exists())// se existe o campo
					{
						if (unit->getOrderTarget() == NULL) // se nunca teve alvo pra coletar
						{
							if (!estaColetanto) //se já não está coletando 
							{
								estaColetanto = true;
								if (!unit->gather(algo)) // começa a coletar
								{
									//handle error
								}
							}
						}
						else
						{
							// se não está coletando e volta a coletar
							if (!estaColetanto || (unit->getOrderTarget() != algo && unit->getOrderTarget()->getType().isMineralField())) 
							{
								estaColetanto = true;
								if (!unit->gather(algo))
								{
									//handle error
								}
							}
						}
					}
					else // se o campo não existe mais, devolve a carga
					{
						devolverCarga = true;
					}
				}
				else // se não tem campo, devolve a carga
				{
					devolverCarga = true;
				}
			} // closure: has no powerup
			
			if (!ReleaseMutex(util->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(30); // Some agents can sleep more than others. 
	}
}
