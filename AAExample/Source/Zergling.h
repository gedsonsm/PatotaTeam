#include "BWAPI.h"
#include <Windows.h>
#include "Util.h"

using namespace BWAPI;
using namespace Filter;

#pragma once
class Zergling
{
public:
	Zergling(Unit unit);
	~Zergling();

private:
	static DWORD WINAPI run(LPVOID param);
};

