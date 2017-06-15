#include "BWAPI.h"
#include <Windows.h>
#include "Utilitarios.h"

using namespace BWAPI;
using namespace Filter;

#pragma once
class GerenciadorAtaque
{
public:
	GerenciadorAtaque();
	~GerenciadorAtaque();

	std::list<BWAPI::Unit> lastAttackList;
	bool isListTargetable(std::list<BWAPI::Unit> list);
	int maxAttackZerglings(Unit u);
	int getListMinZerglings(std::list<BWAPI::Unit> list);

private:
	static DWORD WINAPI run(LPVOID param);
};

