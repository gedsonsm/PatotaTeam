#include "BWAPI.h"
#include <Windows.h>
#include "Utilitarios.h"
#include "GerenciadorColeta.h"

using namespace BWAPI;
using namespace Filter;

#pragma once
class Overlord
{
public:
	Overlord(Unit unit);
	~Overlord();

private:
	static DWORD WINAPI run(LPVOID param);
};

