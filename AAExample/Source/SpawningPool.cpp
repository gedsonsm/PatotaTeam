#include "SpawningPool.h"


SpawningPool::SpawningPool(Unit unit)
{
	CreateThread(NULL, 0, SpawningPool::run, (LPVOID)unit, 0, NULL);
}


SpawningPool::~SpawningPool()
{
}

DWORD WINAPI SpawningPool::run(LPVOID param){
	BWAPI::Unit hq = static_cast<BWAPI::Unit>(param);
	DWORD dwWaitResult;
	Player self = BWAPI::Broodwar->self();

	while (true){

		dwWaitResult = WaitForSingleObject(
			Utils->ghMutex,    // handle to mutex
			100);  // time-out interval

		if (Utils->GameOver || hq == NULL || !hq->exists()) {
			ReleaseMutex(Utils->ghMutex);
			return 0; // end thread
		}
		
		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED)
		{
			
			if (hq->isCompleted()) {
				Utils->construindoPool = false;
				Utils->temPool = true;
				// Release ownership of the mutex object
				if (!ReleaseMutex(Utils->ghMutex))
				{
					// Handle error.
				}
				return 0;
			}

			// Release ownership of the mutex object
			if (!ReleaseMutex(Utils->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(50);
	}
}
