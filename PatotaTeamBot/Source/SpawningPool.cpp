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
			util->ghMutex,    // handle to mutex
			100);  // time-out interval

		if (util->GameOver || hq == NULL || !hq->exists()) {
			ReleaseMutex(util->ghMutex);
			return 0; // end thread
		}
		
		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED)
		{
			
			if (hq->isCompleted()) // se a pool está completa
			{
				util->construindoPool = false; //não está construindo mais
				util->temPool = true; // e agora tem uma pool
				// Release ownership of the mutex object
				if (!ReleaseMutex(util->ghMutex))
				{
					// Handle error.
				}
				return 0;
			}

			// Release ownership of the mutex object
			if (!ReleaseMutex(util->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(50);
	}
}
