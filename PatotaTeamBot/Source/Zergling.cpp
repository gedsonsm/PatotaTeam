#include "Zergling.h"

Zergling::Zergling(Unit unit)
{
	CreateThread(NULL, 0, Zergling::run, (LPVOID)unit, 0, NULL);
}

Zergling::~Zergling()
{
}

DWORD WINAPI Zergling::run(LPVOID param){
	BWAPI::Unit unit = static_cast<BWAPI::Unit>(param);
	DWORD dwWaitResult;
	Player self = BWAPI::Broodwar->self();
	Unit alvo;
	Unit ultimoAlvo = nullptr;
	bool defensor = false;
	bool atacante = false;

	if (util->ataque && util->qtdDefensores < MAX_DEFENSORES) //se a quantidade de defensores estiver baixa o zergling vira defensor
	{
		defensor = true;
		util->qtdDefensores++;
	}
	else 
	{
		util->grupoAtaque.insert(unit); //se não ele vira atacante
		atacante = true;
	}

	while (true){
		dwWaitResult = WaitForSingleObject(
			util->ghMutex,    // handle to mutex
			100);  // time-out interval

		//// If end game, or if it exists (remember to always check)
		if (util->GameOver || unit == NULL || !unit->exists())  {
			if (defensor) {
				util->qtdDefensores--;
			}
			ReleaseMutex(util->ghMutex);
			return 0; // end thread
		} // end thread
		// You can check tons of others things like isStuck, isLockedDown, constructing
		if (!unit->isCompleted() || !unit->isCompleted()){ // You can create it on the onUnitComplete too!
			ReleaseMutex(util->ghMutex);
			Sleep(500);
			continue;
		}

		// You can check tons of others things like isStuck, isLockedDown, constructing
		if (atacante && util->ataque){ // You can create it on the onUnitComplete too!
			ReleaseMutex(util->ghMutex);
			Sleep(500);
			continue;
		}

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) //RAII
		{
			if(defensor && util->ataque) //se for hora de atacar, vai deixar de ser defensor e vai se juntar ao grupo de ataque
			{
				util->grupoAtaque.insert(unit);
				atacante = true;
				util->qtdDefensores--;
				defensor = false;
			} 
			else if (defensor || (atacante && !util->ataque)) // se for defensor ou atacante e não hover ataque
			{
				alvo = util->getAlvoDefesa(unit);
				if (alvo != nullptr && alvo != ultimoAlvo) // vai atacar um inimigo que estiver atacando a base
				{
					unit->attack(alvo);
					ultimoAlvo = alvo;
				}
				else if (unit->getDistance((Position)self->getStartLocation()) > 50 && !unit->isMoving()) // se nao hover inimigos atacando a base
				{
					unit->move((Position)self->getStartLocation());// vai se ficar parado na posição inicial
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
