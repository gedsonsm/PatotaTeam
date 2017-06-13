#include "Ataque.h"

Ataque::Ataque()
{
	CreateThread(NULL, 0, Ataque::run, (LPVOID)this, 0, NULL);
}


Ataque::~Ataque()
{
}

DWORD WINAPI Ataque::run(LPVOID param)
{
	Ataque* squad = static_cast<Ataque*>(param);
	DWORD dwWaitResult;
	Player self = BWAPI::Broodwar->self();
	PositionOrUnit alvo;
	std::list<Unit> listaInimigos;
	bool andando = false;
	const BWAPI::Unitset todosInimigos = Broodwar->enemy()->getUnits();

	while (true){
		dwWaitResult = WaitForSingleObject(
			util->ghMutex,    // handle to mutex
			100);  // time-out interval

		//// If end game, or if it exists (remember to always check)
		if (util->GameOver)  {
			ReleaseMutex(util->ghMutex);
			return 0; // end thread
		} // end thread

		if (!util->atacar && util->qtdZergling <= MIN_ATTACK_ZERGLINGS) {
			ReleaseMutex(util->ghMutex);
			Sleep(500);
			continue;
		}

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) //RAII
		{
			Broodwar->drawTextScreen(20, 20, "entou");
			Unitset::iterator it = util->grupoAtaque.begin();
			while (it != util->grupoAtaque.end()) {
				if (!(*it)->exists()) {
					it == util->grupoAtaque.erase(it);
				}
				else {
					it++;
				}
			}

			if (util->atacar && util->grupoAtaque.size() <= MIN_ATTACK_ZERGLINGS){
				util->grupoAtaque.move((Position)self->getStartLocation());
				util->atacar = false;
			}
			else if (util->qtdZergling >= MAX_ZERGLING)
			{
				util->atacar = true;
				for each (Unit u in todosInimigos)
				{
					if (u->getType() != UnitTypes::Zerg_Overlord && u->exists() && u->isTargetable()) 
					{
						listaInimigos.push_back(u);
					}
				}

				if (listaInimigos.empty()) 
				{
					if (!andando && util->localInimigo != (Position)self->getStartLocation()) 
					{
						util->grupoAtaque.move(util->localInimigo);
						andando = true;
					}
				}
				else 
				{
					andando = false;
					util->grupoAtaque.attack(*listaInimigos.begin());
					
				}
			}

			if (!ReleaseMutex(util->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(30);
	}
}