#include "Utilitarios.h"

using namespace BWAPI;
using namespace Filter;

Utilitarios* Utilitarios::instance;

Utilitarios::Utilitarios()
{
	GameOver = false;
	temPool = false;
	construindoPool = false;
	construirPool = false;
	subistituiDronePool = false;
	treinandoOverlord = false;
	qtdLarva = 0;
	qtdZergling = 0;
	qtdDrone = 0;

	// Create a mutex with no initial owner
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex
}


Utilitarios::~Utilitarios()
{
}

Utilitarios* Utilitarios::getInstance() {
	if (instance == NULL) {
		instance = new Utilitarios();
	}
	return instance;
}
