#include "Util.h"

//Defines
#define GATHER_TIME 600

#define Mineral MineralManager::getInstancia()
#define MineralQueue std::set<BWAPI::Unit, MineralManager::CompareDistance> 

#pragma once

class MineralManager
{
public:

	struct CompareDistance
	{
		bool operator () (const BWAPI::Unit & p_lhs, const BWAPI::Unit & p_rhs);
	};

	MineralQueue mineralList;

	static MineralManager* getInstancia();

	BWAPI::Unit getMineralField(BWAPI::Unit);
	void leaveQueue(BWAPI::Unit);

private:
	MineralManager();
	~MineralManager();
	std::atomic<int*> mineralQueueCount;
	static MineralManager* instance;
	MineralQueue getMineralFields();
};

