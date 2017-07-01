#include "BWAPI.h"
#include <Windows.h>
#include "Utilitarios.h"

using namespace BWAPI;
using namespace Filter;
#pragma once
class Hatchery
{
public:
	Hatchery(Unit unit);
	~Hatchery();

private:
	static DWORD WINAPI run(LPVOID param);
};

