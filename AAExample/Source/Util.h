#include <atomic>
#include <iostream>
#include <Windows.h>
#include <BWAPI.h>

using namespace BWAPI;

//Defines
#define MIN_DRONES 2
#define MAX_DRONES 5

#define MAX_DEFENSORES 4
#define MIN_ATACANTES 8
#define MIN_ATACANTES_PARAR_ATAQUE 1
#define MIN_ZERGLING (MAX_DEFENSORES + MIN_ATACANTES)

#define MAX_ZERGLING (2*MIN_ZERGLING)

#define Utils Util::getInstancia()
#define UnitGroup std::set<BWAPI::Unit>
#define UnitList std::list<BWAPI::Unit>

#pragma once
class Util
{
public:
	std::atomic<bool> GameOver;
	std::atomic<bool> temPool;
	std::atomic<bool> construindoPool;
	std::atomic<bool> subistituiDronePool;
	std::atomic<bool> startScout;
	std::atomic<bool> rush;
	std::atomic<bool> construirPool;

	//Contadores de unidades
	std::atomic<int> qtdLarva;
	std::atomic<int> qtdZergling;
	std::atomic<int> qtdDrone;
	std::atomic<int> qtdDefensores;
	
	HANDLE ghMutex;
	static Util* getInstancia();

	BWAPI::Unitset grupoAtaque;

	BWAPI::PositionOrUnit getAlvo();
	Unit Util::getAlvoDefesa(Unit unit);
	UnitList getListaAlvos();
	BWAPI::Position baseInimigo;
	int Util::getPrioridade(BWAPI::Unit unit);

private:
	~Util();
	Util();
	static Util* instance;
	UnitGroup getInimigos();
	int getPrioridade(BWAPI::UnitType unit);
	BWAPI::Position getBaseInimigo();
};

