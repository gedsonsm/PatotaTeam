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

#define util Utilitarios::getInstancia()

#pragma once
class Utilitarios
{
public:
	std::atomic<bool> GameOver;
	std::atomic<bool> temPool;
	std::atomic<bool> construindoPool;
	std::atomic<bool> substituiDrone;
	std::atomic<bool> startScout;
	std::atomic<bool> rush;
	std::atomic<bool> construirPool;

	//Contadores de unidades
	std::atomic<int> qtdLarva;
	std::atomic<int> qtdZergling;
	std::atomic<int> qtdDrone;
	std::atomic<int> qtdDefensores;
	
	HANDLE ghMutex;
	static Utilitarios* getInstancia();

	BWAPI::Unitset grupoAtaque;

	BWAPI::PositionOrUnit getAlvo();
	Unit Utilitarios::getAlvoDefesa(Unit unit);
	std::list<BWAPI::Unit> getListaAlvos();
	BWAPI::Position baseInimigo;
	int Utilitarios::getPrioridade(BWAPI::Unit unit);
	BWAPI::Position getBaseInimigo();
private:
	~Utilitarios();
	Utilitarios();
	static Utilitarios* instance;
	std::set<BWAPI::Unit> getInimigos();
	int getPrioridade(BWAPI::UnitType unit);
	
};

