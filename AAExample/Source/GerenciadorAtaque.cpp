#include "GerenciadorAtaque.h"


GerenciadorAtaque::GerenciadorAtaque()
{
	CreateThread(NULL, 0, GerenciadorAtaque::run, (LPVOID)this, 0, NULL);
}


GerenciadorAtaque::~GerenciadorAtaque()
{
}

bool GerenciadorAtaque::isListTargetable(std::list<BWAPI::Unit> list) {
	std::list<BWAPI::Unit>::iterator it = list.begin();
	while (it != list.end()){
		if ((*it)->isTargetable()) {
			return true;
		}
		it++;
	}
	return false;
}

int GerenciadorAtaque::getListMinZerglings(std::list<BWAPI::Unit> list) {
	int counter = 0;
	std::list<BWAPI::Unit>::iterator it = list.begin();
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

int GerenciadorAtaque::maxAttackZerglings(Unit u) {
	UnitType unit = u->getType();
	if (unit == BWAPI::UnitTypes::Terran_SCV || unit == BWAPI::UnitTypes::Zerg_Drone || unit == BWAPI::UnitTypes::Protoss_Probe || unit == UnitTypes::Zerg_Larva){
		return 5;
	}
	else {
		return 0;
	}
}

DWORD WINAPI GerenciadorAtaque::run(LPVOID param){
	GerenciadorAtaque* squad = static_cast<GerenciadorAtaque*>(param);
	DWORD dwWaitResult;
	Player self = BWAPI::Broodwar->self();
	PositionOrUnit target;
	PositionOrUnit lastTarget = nullptr;
	std::list<BWAPI::Unit> list;
	bool moving = false;

	while (true){
		dwWaitResult = WaitForSingleObject(
			util->ghMutex,    // handle to mutex
			100);  // time-out interval

		//// If end game, or if it exists (remember to always check)
		if (util->GameOver)  {
			ReleaseMutex(util->ghMutex);
			return 0; // end thread
		} // end thread

		if (!util->rush && util->qtdZergling <= MIN_ATACANTES) {
			ReleaseMutex(util->ghMutex);
			Sleep(500);
			continue;
		}

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) //RAII
		{
			Unitset::iterator it = util->grupoAtaque.begin();
			while (it != util->grupoAtaque.end()) {
				if (!(*it)->exists()) {
					it = util->grupoAtaque.erase(it);
				}
				else {
					it++;
				}
			}

			if (util->rush && util->grupoAtaque.size() <= MIN_ATACANTES_PARAR_ATAQUE){
				util->grupoAtaque.move((Position)self->getStartLocation());
				util->rush = false;
			} else {
				util->rush = true;
				list = util->getListaAlvos();
				if (list.empty()) {
					if (!moving && util->baseInimigo != (Position)self->getStartLocation()) {
						util->grupoAtaque.move(util->baseInimigo);
						moving = true;
					}
				} else if (list != squad->lastAttackList && squad->isListTargetable(list)) {
					moving = false;
					squad->lastAttackList = list;

					if (util->grupoAtaque.size() < 10) {
						util->grupoAtaque.attack(*list.begin());
					}
					else {
						std::list<BWAPI::Unit>::iterator it3 = list.begin();
						Unitset::iterator it2 = util->grupoAtaque.begin();
						while (it3 != list.end() && it2 != util->grupoAtaque.end()) {
							if ((*it3)->exists() && (*it3)->isTargetable()) {
								int max = squad->maxAttackZerglings(*it3);
								if (max == 0) {
									Unitset temp2;
									temp2.insert(it2, util->grupoAtaque.end());
									temp2.attack(*it3);
									it3 = list.end();
								}
								else {
									Unitset temp;
									for (int i = 0; i < max && it2 != util->grupoAtaque.end(); i++) {
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


			if (!ReleaseMutex(util->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(30);
	}
}
