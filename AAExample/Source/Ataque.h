#include "BWAPI.h"
#include <Windows.h>
#include "Utilitarios.h"


using namespace BWAPI;
using namespace Filter;

#pragma once
class Ataque
{
public:
	Ataque();
	~Ataque();



private:
	static DWORD WINAPI run(LPVOID param);
};

