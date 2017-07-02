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
	Player self = BWAPI::Broodwar->self();
	BWAPI::Position localInicial = (Position)self->getStartLocation();
	DWORD dwWaitResult;
	Unit algo = nullptr;
	bool devolverCarga = true;
	bool estaColetanto = false;


	int i = 0;
	int j = 0;
	bool canto = false;
	bool outroLado = false;
	int contCanto = 0;
	int cont = 0;

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
				Broodwar->drawTextScreen(100, 50, "FIRST");
				TilePosition localConstrucao = Broodwar->getBuildLocation(UnitTypes::Zerg_Spawning_Pool, unit->getTilePosition());
				if (localConstrucao) //se tem um lugar pra costruie
				{
					unit->build(UnitTypes::Zerg_Spawning_Pool, localConstrucao);
					util->construirPool = false;
					util->construindoPool = true;
					//util->substituiDrone = true;
				}
				ReleaseMutex(util->ghMutex); //libera o mutex, pois o drone vai deixar de existir
				return 0;
			}
			unitTarget = util->getAlvoDefesa(unit);
			if (unitTarget != nullptr)  // se existe um algo que está atacando
			{
				//Broodwar->drawTextScreen(100, 50, "SECOND");
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
				//Broodwar->drawTextScreen(100, 50, "THIRD");
				estaColetanto = false; //para de coletar
				//Broodwar->drawTextScreen(100, 50, "22222");
				if (!devolverCarga)  // devolve a carga
				{
					//Broodwar->drawTextScreen(100, 50, "11111");
					unit->returnCargo();
					Mineral->leaveQueue(algo);
					algo = nullptr;
					devolverCarga = true;
				}
			}
			else if (!unit->getPowerUp()) //não sei porque, mas tive dificuldade de tirar esse if, deve ser algo da api
			{
				//Broodwar->drawTextScreen(100, 50, "FOURTH");
				if (devolverCarga) //se já devolveu
				{
					//Broodwar->drawTextScreen(100, 50, "33333");
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
									//continue;
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
									//continue;
								}
							}
						}
					}
					else // se o campo não existe mais, devolve a carga
					{
						//Broodwar->drawTextScreen(100, 50, "ACABOU %d",acabou);
						//acabou++;
						devolverCarga = true;
					}
				}
				else // se não tem campo, devolve a carga
				{
					//	Broodwar->drawTextScreen(100, 50, "PAROU");
					devolverCarga = true;
				}
			} // closure: has no powerup
			if (unit->isIdle())		//Se ele não esta fazendo nada
			{
				if (!unit->isMoving())					//Funcao de movimentação de GEDSON com uma modificacao
				{
					if (localInicial.y > 1000)
					{
						if (!outroLado)
						{
							if (localInicial.x + i <= 200)
								canto = true;
							if (!canto)
								i -= 100;
							else if (canto && contCanto < 6)
							{
								j -= 100;
								contCanto++;
							}
							else
							{
								outroLado = true;
								canto = false;
								contCanto = 0;
							}
						}
						else
						{
							if (localInicial.x + i >= 1700) //Modificação, quando o caminho acaba, o drone volta pra base e recomeca o caminho
							{
								devolverCarga = true;
								estaColetanto = false;
								i = 0;
								j = 0;
								canto = false;
								outroLado = false;
								contCanto = 0;
								cont = cont + 20;
							}
							if (!canto)
								i += 100;
						}
					}
					if (localInicial.y <= 1000)
					{
						if (!outroLado)
						{
							if (localInicial.x + i >= 1700)
								canto = true;
							if (!canto)
								i += 100;
							else if (canto && contCanto < 6)
							{
								j += 100;
								contCanto++;
							}
							else
							{
								outroLado = true;
								canto = false;
								contCanto = 0;
							}
						}
						else
						{
							if (localInicial.x + i <= 200)		//Modificação, quando o caminho acaba, o drone volta pra base e recomeca o caminho
							{
								
								devolverCarga = true;
								estaColetanto = false;
								i = 0;
								j = 0;
								canto = false;
								outroLado = false;
								contCanto = 0;
								//cont = cont + 20;
							}
							if (!canto)
								i -= 100;
						}
					}
					BWAPI::Position pos(localInicial.x + i, localInicial.y + j);
					unit->move(pos);
					//devolverCarga = true;
				}
			}
			if (!ReleaseMutex(util->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(30); // Some agents can sleep more than others. 
	}
}