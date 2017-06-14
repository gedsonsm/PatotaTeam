#include "BWAPI.h"
#include <Windows.h>
#include "Util.h"
#include "MineralManager.h"

using namespace BWAPI;
using namespace Filter;

#pragma once
class ZergDrone
{
public:
	ZergDrone(Unit unit);
	~ZergDrone();

private:
	static DWORD WINAPI run(LPVOID param);
};

