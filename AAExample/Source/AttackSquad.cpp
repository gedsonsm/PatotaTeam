#include "AttackSquad.h"


AttackSquad::AttackSquad()
{
	CreateThread(NULL, 0, AttackSquad::run, (LPVOID)this, 0, NULL);
}


AttackSquad::~AttackSquad()
{
}

bool AttackSquad::isListTargetable(UnitList list) {
	UnitList::iterator it = list.begin();
	while (it != list.end()){
		if ((*it)->isTargetable()) {
			return true;
		}
		it++;
	}
	return false;
}

int AttackSquad::getListMinZerglings(UnitList list) {
	int counter = 0;
	UnitList::iterator it = list.begin();
	while (it != list.end()){
		if ((*it)->getType() == UnitTypes::Terran_Marine) {
			counter += 2;
		}
		else if ((*it)->getType() == UnitTypes::Terran_Bunker) {
			counter += 8;
		}
		it++;
	}
	return counter;
}

int AttackSquad::maxAttackZerglings(Unit u) {
	UnitType unit = u->getType();
	if (unit == BWAPI::UnitTypes::Terran_SCV || unit == BWAPI::UnitTypes::Zerg_Drone || unit == BWAPI::UnitTypes::Protoss_Probe || unit == UnitTypes::Zerg_Larva){
		return 5;
	}
	else {
		return 0;
	}
}

DWORD WINAPI AttackSquad::run(LPVOID param){
	AttackSquad* squad = static_cast<AttackSquad*>(param);
	DWORD dwWaitResult;
	Player self = BWAPI::Broodwar->self();
	PositionOrUnit target;
	PositionOrUnit lastTarget = nullptr;
	UnitList list;
	bool moving = false;

	while (true){
		dwWaitResult = WaitForSingleObject(
			Utils->ghMutex,    // handle to mutex
			100);  // time-out interval

		//// If end game, or if it exists (remember to always check)
		if (Utils->GameOver)  {
			ReleaseMutex(Utils->ghMutex);
			return 0; // end thread
		} // end thread

		if (!Utils->rush && Utils->qtdZergling <= MIN_ATACANTES) {
			ReleaseMutex(Utils->ghMutex);
			Sleep(500);
			continue;
		}

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) //RAII
		{
			Unitset::iterator it = Utils->grupoAtaque.begin();
			while (it != Utils->grupoAtaque.end()) {
				if (!(*it)->exists()) {
					it = Utils->grupoAtaque.erase(it);
				}
				else {
					it++;
				}
			}

			if (Utils->rush && Utils->grupoAtaque.size() <= MIN_ATACANTES_PARAR_ATAQUE){
				Utils->grupoAtaque.move((Position)self->getStartLocation());
				Utils->rush = false;
			} else {
				Utils->rush = true;
				list = Utils->getListaAlvos();
				if (list.empty()) {
					if (!moving && Utils->baseInimigo != (Position)self->getStartLocation()) {
						Utils->grupoAtaque.move(Utils->baseInimigo);
						moving = true;
					}
				} else if (list != squad->lastAttackList && squad->isListTargetable(list)) {
					moving = false;
					squad->lastAttackList = list;

					if (Utils->grupoAtaque.size() < 10) {
						Utils->grupoAtaque.attack(*list.begin());
					}
					else {
						UnitList::iterator it3 = list.begin();
						Unitset::iterator it2 = Utils->grupoAtaque.begin();
						while (it3 != list.end() && it2 != Utils->grupoAtaque.end()) {
							if ((*it3)->exists() && (*it3)->isTargetable()) {
								int max = squad->maxAttackZerglings(*it3);
								if (max == 0) {
									Unitset temp2;
									temp2.insert(it2, Utils->grupoAtaque.end());
									temp2.attack(*it3);
									it3 = list.end();
								}
								else {
									Unitset temp;
									for (int i = 0; i < max && it2 != Utils->grupoAtaque.end(); i++) {
										temp.insert(*it2);
										it2++;
									}
									temp.attack(*it3);
									it3++;
								}
							}
							else {
								it3++;
							}
						}
					}
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
