#include "Overlord.h"

Overlord::Overlord(Unit unit)
{
	CreateThread(NULL, 0, Overlord::run, (LPVOID)unit, 0, NULL);
}

Overlord::~Overlord()
{
}

DWORD WINAPI Overlord::run(LPVOID param){
	BWAPI::Unit unit = static_cast<BWAPI::Unit>(param);

	DWORD dwWaitResult;
	Player self = BWAPI::Broodwar->self();
	PositionOrUnit alvo;
	PositionOrUnit ultimoAlvo = nullptr;
	TilePosition::list bases = Broodwar->getStartLocations();
	TilePosition::list::iterator it = bases.begin();
	TilePosition::list::iterator lastIt = bases.begin();
	BWAPI::Position localInicial = (Position)self->getStartLocation();
	BWAPI::Unit unidadesIniciais = BWAPI::Broodwar->getClosestUnit((Position)self->getStartLocation(), BWAPI::Filter::IsResourceDepot); 
	BWAPI::Unitset mineriosIniciais = unidadesIniciais->getUnitsInRadius(1024, BWAPI::Filter::IsMineralField);
	BWAPI::Unitset mineraisAoRedor;
	

	bool scouting = false;
	int i = 0;
	int j = 0;
	bool canto = false;
	bool direita = false;
	bool esquerda = false;
	bool cima = false;
	bool baixo = false;
	bool outroLado = false;
	int contCanto = 0;
	bool paraScout = true;
	
	if (util->scout) // vai na base inimiga
	{
		paraScout = false;
		util->scout = false;
	}
	while (true)
	{
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

		if ((dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) && unit->getType() == UnitTypes::Zerg_Overlord) //RAII
		{
			if (unit->isUnderAttack() || paraScout) // volta pra base se sofre um ataque
			{
				unit->move((Position)self->getStartLocation());
			}
			else if (!unit->isMoving() && !paraScout)
			{
				if (localInicial.y > 1000)
				{
					if (!outroLado)
					{
						if (localInicial.x + i <= 200)
						{
							canto = true;
						}
						if (!canto)
						{
							i -= 100;
						}
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
						if (localInicial.x + i >= 1700)
						{
							paraScout = true;
						}
						if (!canto)
						{
							i += 100;
						}
					}
				}
				else if (localInicial.y <= 1000)
				{
					if (!outroLado)
					{
						if (localInicial.x + i >= 1700)
						{
							canto = true;
						}
						if (!canto)
						{
							i += 100;
						}
						else if (canto && contCanto < 6	)
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
						if (localInicial.x + i <= 200)
						{
							paraScout = true;
						}
						if (!canto)
						{
							i -= 100;
						}
					}
				}
				BWAPI::Position pos(localInicial.x+i, localInicial.y+j);
				unit->move(pos);
			}
			Broodwar->drawTextScreen(100, 50, "%d -- %d", localInicial.x + i, localInicial.y + j);
		
			if (!ReleaseMutex(util->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(30);
	}
}
