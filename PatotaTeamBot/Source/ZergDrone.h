#include "BWAPI.h"
#include <Windows.h>
#include "Utilitarios.h"
#include "GerenciadorColeta.h"

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

