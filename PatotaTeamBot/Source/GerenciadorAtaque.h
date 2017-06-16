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

	std::list<BWAPI::Unit> ultimaListaAtaque;
	bool listaSelecionavel(std::list<BWAPI::Unit> list);
	int mixAtaqueZerglings(Unit u);

private:
	static DWORD WINAPI run(LPVOID param);
};

