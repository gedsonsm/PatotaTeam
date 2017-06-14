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
	Unit target;
	Unit lastTarget = nullptr;
	bool defenseSquad = false;
	bool attackSquad = false;

	if (Utils->rush && Utils->qtdDefensores < MAX_DEFENSORES) {
		defenseSquad = true;
		Utils->qtdDefensores++;
	}
	else {
		Utils->grupoAtaque.insert(unit);
		attackSquad = true;
	}

	while (true){
		dwWaitResult = WaitForSingleObject(
			Utils->ghMutex,    // handle to mutex
			100);  // time-out interval

		//// If end game, or if it exists (remember to always check)
		if (Utils->GameOver || unit == NULL || !unit->exists())  {
			if (defenseSquad) {
				Utils->qtdDefensores--;
			}
			ReleaseMutex(Utils->ghMutex);
			return 0; // end thread
		} // end thread
		// You can check tons of others things like isStuck, isLockedDown, constructing
		if (!unit->isCompleted() || !unit->isCompleted()){ // You can create it on the onUnitComplete too!
			ReleaseMutex(Utils->ghMutex);
			Sleep(500);
			continue;
		}

		// You can check tons of others things like isStuck, isLockedDown, constructing
		if (attackSquad && Utils->rush){ // You can create it on the onUnitComplete too!
			ReleaseMutex(Utils->ghMutex);
			Sleep(500);
			continue;
		}

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) //RAII
		{
			if(defenseSquad && !Utils->rush) {
				Utils->grupoAtaque.insert(unit);
				attackSquad = true;
				Utils->qtdDefensores--;
				defenseSquad = false;
			} else if (defenseSquad || (attackSquad && !Utils->rush)) {
				target = Utils->getAlvoDefesa(unit);
				if (target != nullptr && target != lastTarget) {
					unit->attack(target);
					lastTarget = target;
				}
				else if (unit->getDistance((Position)self->getStartLocation()) > 50 && !unit->isMoving()) {
					unit->move((Position)self->getStartLocation());
				}
			}

			if (!ReleaseMutex(Utils->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(30);
	}
}
