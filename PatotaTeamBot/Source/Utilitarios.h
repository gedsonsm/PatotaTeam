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
	std::atomic<bool> GameOver; //já acabou jessica?
	std::atomic<bool> temPool; 
	std::atomic<bool> construindoPool;
	std::atomic<bool> substituiDrone;
	std::atomic<bool >construindoExtractor;
	std::atomic<bool> construirExtractor;
	std::atomic<bool> scout;
	std::atomic<bool> ataque;
	std::atomic<bool> construirPool;

	//Contadores de unidades
	std::atomic<int> qtdLarva;
	std::atomic<int> qtdZergling;
	std::atomic<int> qtdDrone;
	std::atomic<int> qtdDefensores;
	std::atomic<bool> aux;
	std::atomic<bool> fazDroneTest;
	BWAPI::Unitset gasAoRedor;
	std::atomic<bool> coletaGas;
	
	HANDLE ghMutex;
	
	BWAPI::Position baseInimigo; // guarda a posição da base inimiga
	BWAPI::Unitset grupoAtaque; // aqui fica todos os zerglings que vão fazer um ataque

	static Utilitarios* getInstancia(); // criar uma instncia da classe
	BWAPI::PositionOrUnit getAlvo(); // pega um algo para atacar
	Unit Utilitarios::getAlvoDefesa(Unit unit); //pega um alvo para atacar durante uma defesa
	std::list<BWAPI::Unit> getListaAlvos(); // pega todos os algos proximos
	int Utilitarios::getPrioridade(BWAPI::Unit unit); // seleciona a prioridade de ataque
	BWAPI::Position getBaseInimigo(); // pega posição da base inimiga

private:
	~Utilitarios();
	Utilitarios();
	static Utilitarios* instanciaUtil; //instancia da classe Utilitarios
	std::set<BWAPI::Unit> getInimigos(); //pega todos os tipos de inimigos do oponente
	int getPrioridade(BWAPI::UnitType unit); //sobrecarga
	
};

