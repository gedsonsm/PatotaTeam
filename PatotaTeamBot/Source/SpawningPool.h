#include "BWAPI.h"
#include <Windows.h>
#include "Utilitarios.h"

using namespace BWAPI;
using namespace Filter;

#pragma once
class SpawningPool
{
public:
	SpawningPool(Unit unit);
	~SpawningPool();

private:
	static DWORD WINAPI run(LPVOID param);
};

