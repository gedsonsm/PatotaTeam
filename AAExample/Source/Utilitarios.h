#include <atomic>
#include <iostream>
#include <Windows.h>
#include <BWAPI.h>

using namespace BWAPI;

//Defines
#define MIN_DRONES 2
#define MAX_DRONES 5

#define MAX_DEFENSE_ZERGLINGS 2
#define MIN_ATTACK_ZERGLINGS 4
#define MIN_ZERGLING (MAX_DEFENSE_ZERGLINGS + MIN_ATTACK_ZERGLINGS)

#define MAX_ZERGLING (2*MIN_ZERGLING)

#define util Utilitarios::getInstance()
#define UnitGroup std::set<BWAPI::Unit>
#define UnitList std::list<BWAPI::Unit>

#pragma once
class Utilitarios
{
public:
	std::atomic<bool> GameOver;
	std::atomic<bool> temPool;
	std::atomic<bool> construindoPool;
	std::atomic<bool> construirPool;
	std::atomic<bool> subistituiDronePool;
	//Contadores de unidades
	std::atomic<int> qtdLarva;
	std::atomic<int> qtdZergling;
	std::atomic<int> qtdDrone;
	
	HANDLE ghMutex;
	static Utilitarios* getInstance();

private:
	~Utilitarios();
	Utilitarios();
	static Utilitarios* instance;
};

