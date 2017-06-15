#include "Utilitarios.h"

//Defines
#define GATHER_TIME 600

#define Mineral GerenciadorColeta::getInstancia()
#define MineralQueue std::set<BWAPI::Unit, GerenciadorColeta::CompareDistance> 

#pragma once

class GerenciadorColeta
{
public:

	struct CompareDistance
	{
		bool operator () (const BWAPI::Unit & p_lhs, const BWAPI::Unit & p_rhs);
	};

	MineralQueue mineralList;

	static GerenciadorColeta* getInstancia();

	BWAPI::Unit getMineralField(BWAPI::Unit);
	void leaveQueue(BWAPI::Unit);

private:
	GerenciadorColeta();
	~GerenciadorColeta();
	std::atomic<int*> mineralQueueCount;
	static GerenciadorColeta* instance;
	MineralQueue getMineralFields();
};

