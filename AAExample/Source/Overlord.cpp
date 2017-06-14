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
	PositionOrUnit target;
	PositionOrUnit lastTarget = nullptr;
	TilePosition::list bases = Broodwar->getStartLocations();
	TilePosition::list::iterator it = bases.begin();
	TilePosition::list::iterator lastIt = bases.begin();

	bool scouting = false;

	while (true){
		dwWaitResult = WaitForSingleObject(
			Utils->ghMutex,    // handle to mutex
			100);  // time-out interval

		//// If end game, or if it exists (remember to always check)
		if (Utils->GameOver || unit == NULL || !unit->exists())  {
			ReleaseMutex(Utils->ghMutex);
			return 0; // end thread
		} // end thread
		// You can check tons of others things like isStuck, isLockedDown, constructing
		if (!unit->isCompleted() || !unit->isCompleted()){ // You can create it on the onUnitComplete too!
			ReleaseMutex(Utils->ghMutex);
			Sleep(500);
			continue;
		}

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) //RAII
		{
			if (!Utils->startScout) {
				Utils->startScout = true;

				if (Utils->baseInimigo == (Position)self->getStartLocation() && bases.size() > 2) {
					for each (TilePosition p in bases)
					{
						if (p != Broodwar->self()->getStartLocation())
							unit->move((Position)p, true);
					}
					scouting = true;
				}
			}

			if (scouting) {
				if (unit->isUnderAttack()) {
					int distance = -1;
					Position base;
					for each (TilePosition p in bases) {
						int distTemp = unit->getDistance((Position)p);
						if (p != Broodwar->self()->getStartLocation()){
							if (distance == -1 || distance > distTemp) {
								distance = distTemp;
								base = (Position)p;
							}
						}
					}

					Utils->baseInimigo = base;
					unit->move((Position)self->getStartLocation());
					scouting = false;
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
