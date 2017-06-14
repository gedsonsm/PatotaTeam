#include "BWAPI.h"
#include <Windows.h>
#include "Util.h"

using namespace BWAPI;
using namespace Filter;

#pragma once
class AttackSquad
{
public:
	AttackSquad();
	~AttackSquad();

	UnitList lastAttackList;
	bool isListTargetable(UnitList list);
	int maxAttackZerglings(Unit u);
	int getListMinZerglings(UnitList list);

private:
	static DWORD WINAPI run(LPVOID param);
};

