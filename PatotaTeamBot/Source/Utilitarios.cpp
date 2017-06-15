#include "Utilitarios.h"

using namespace BWAPI;
using namespace Filter;

Utilitarios* Utilitarios::instance;

Utilitarios::Utilitarios()
{
	GameOver = false;
	temPool = false;
	construindoPool = false;
	substituiDrone = false;
	startScout = false;
	rush = false;
	construirPool = false;

	//Inicializando contadores
	qtdLarva = 0;
	qtdZergling = 0;
	qtdDrone = 0;
	qtdDefensores = 0;

	// Create a mutex with no initial owner
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	baseInimigo = getBaseInimigo();
}


Utilitarios::~Utilitarios()
{
}

Utilitarios* Utilitarios::getInstancia()
{
	if (instance == NULL) 
	{
		instance = new Utilitarios();
	}
	return instance;
}

PositionOrUnit Utilitarios::getAlvo()
{
	std::set<BWAPI::Unit> enemies = getInimigos();
	BWAPI::PositionOrUnit target = baseInimigo;
	int priority = 99;

	for each (BWAPI::Unit u in enemies)
	{
		if (u->exists() && u->isTargetable()) 
		{
			if (u->isAttacking())
			{
				target = u;
				break;
			}
			else
			{
				int unitPriority = getPrioridade(u->getType());
				if (unitPriority < priority)
				{
					priority = unitPriority;
					target = u;
				}
			}
		}
	}

	return target;
}

Unit Utilitarios::getAlvoDefesa(Unit unit)
{
	std::list<BWAPI::Unit> list = getListaAlvos();
	std::list<BWAPI::Unit>::iterator it = list.begin();

	while (it != list.end()) 
	{
		if ((*it)->getDistance(unit) < 100) 
		{
			return *it;
		}
		it++;
	}

	return nullptr;
}

int Utilitarios::getPrioridade(BWAPI::UnitType unit)
{
	if (unit == BWAPI::UnitTypes::Terran_Marine || unit == BWAPI::UnitTypes::Zerg_Zergling || unit == BWAPI::UnitTypes::Protoss_Zealot)
		return 1;
	else if (unit == BWAPI::UnitTypes::Terran_SCV)
		return 2;
	else if (unit == BWAPI::UnitTypes::Terran_Bunker || unit == BWAPI::UnitTypes::Zerg_Drone || unit == BWAPI::UnitTypes::Protoss_Probe)
		return 3;
	else if (unit == BWAPI::UnitTypes::Terran_Barracks || unit == BWAPI::UnitTypes::Zerg_Spawning_Pool || unit == BWAPI::UnitTypes::Protoss_Pylon)
		return 4;
	else if (unit == BWAPI::UnitTypes::Terran_Command_Center || unit == BWAPI::UnitTypes::Zerg_Creep_Colony || unit == BWAPI::UnitTypes::Protoss_Nexus)
		return 5;
	else if (unit == BWAPI::UnitTypes::Terran_Supply_Depot || unit == BWAPI::UnitTypes::Protoss_Gateway)
		return 6;
	else
		return 7;
}

std::set<BWAPI::Unit> Utilitarios::getInimigos()
{
	const BWAPI::Unitset units = Broodwar->enemy()->getUnits();
	std::set<BWAPI::Unit> enemies;
	enemies.insert(units.begin(), units.end());
	return enemies;
}

Position Utilitarios::getBaseInimigo()
{
	TilePosition::list bases = Broodwar->getStartLocations();
	Position ret;
	if (bases.size() <= 2)
	{
		for each (TilePosition p in bases)
		{
			if (p != Broodwar->self()->getStartLocation())
			{
				ret = (Position)p;
			}
		}
	}
	else
	{
		ret = (Position)Broodwar->self()->getStartLocation();
	}
	return ret;

}

bool compararPrioridade(Unit& u1, Unit& u2) 
{
	bool ret = false;
	if (util->getPrioridade(u1) < util->getPrioridade(u2))
	{
		ret = true;
	}
	return ret;
}

std::list<BWAPI::Unit> Utilitarios::getListaAlvos() 
{
	const BWAPI::Unitset units = Broodwar->enemy()->getUnits();
	std::list<Unit> enemies;
	for each (Unit u in units)
	{
		if (u->getType() != UnitTypes::Zerg_Overlord && u->exists() && u->isTargetable())
		{
			enemies.push_back(u);
		}
	}
	enemies.sort(compararPrioridade);

	return enemies;
}

int Utilitarios::getPrioridade(BWAPI::Unit unit)
{
	if (!unit->isAttacking()) 
	{
		return getPrioridade(unit->getType());
	}
	return 0;
}