#include "Overlord.h"

Overlord::Overlord(Unit unit)
{
	CreateThread(NULL, 0, Overlord::run, (LPVOID)unit, 0, NULL);
}

Overlord::~Overlord()
{
}

DWORD WINAPI Overlord::run(LPVOID param)
{
	BWAPI::Unit unit = static_cast<BWAPI::Unit>(param);
	DWORD dwWaitResult;
	Player self = BWAPI::Broodwar->self();

	bool scouting = false;

	while (true){
		dwWaitResult = WaitForSingleObject(
			util->ghMutex,    // handle to mutex
			100);  // time-out interval

		//// If end game, or if it exists (remember to always check)
		if (util->GameOver || unit == NULL || !unit->exists())  
		{
			ReleaseMutex(util->ghMutex);
			return 0; // end thread
		} // end thread
		// You can check tons of others things like isStuck, isLockedDown, constructing
		if (!unit->isCompleted() || !unit->isCompleted())
		{ // You can create it on the onUnitComplete too!
			ReleaseMutex(util->ghMutex);
			Sleep(500);
			continue;
		}

		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) //RAII
		{
			if (unit->getDistance((Position)self->getStartLocation()) > 50 && !unit->isMoving())
			{
				unit->move((Position)self->getStartLocation());
			}

			if (!ReleaseMutex(util->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(30);
	}
}
