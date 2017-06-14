#include "Util.h"

using namespace BWAPI;
using namespace Filter;

Util* Util::instance;

Util::Util()
{
	GameOver = false;
	temPool = false;
	construindoPool = false;
	subistituiDronePool = false;
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


Util::~Util()
{
}

Util* Util::getInstancia() {
	if (instance == NULL) {
		instance = new Util();
	}
	return instance;
}

PositionOrUnit Util::getAlvo()
{
	UnitGroup enemies = getInimigos();
	BWAPI::PositionOrUnit target = baseInimigo;
	int priority = 99;

	for each (BWAPI::Unit u in enemies)
	{
		if (!u->exists() || !u->isTargetable()) {
			continue;
		}

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

	return target;
}

Unit Util::getAlvoDefesa(Unit unit)
{
	UnitList list = getListaAlvos();
	UnitList::iterator it = list.begin();

	while (it != list.end()) {
		if ((*it)->getDistance(unit) < 100) {
			return *it;
		}
		it++;
	}

	return nullptr;
}

int Util::getPrioridade(BWAPI::UnitType unit)
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

UnitGroup Util::getInimigos()
{
	const BWAPI::Unitset units = Broodwar->enemy()->getUnits();
	UnitGroup enemies;
	enemies.insert(units.begin(), units.end());
	return enemies;
}

Position Util::getBaseInimigo()
{
	TilePosition::list bases = Broodwar->getStartLocations();
	if (bases.size() > 2) {
		return (Position)Broodwar->self()->getStartLocation();
	}
	else {
		for each (TilePosition p in bases)
		{
			if (p != Broodwar->self()->getStartLocation())
				return (Position)p;
		}
	}
}

bool comparePriorities(Unit& u1, Unit& u2) {
	if (Utils->getPrioridade(u1) < Utils->getPrioridade(u2)) {
		return true;
	}
	else {
		return false;
	}
}

UnitList Util::getListaAlvos() {
	const BWAPI::Unitset units = Broodwar->enemy()->getUnits();
	std::list<Unit> enemies;
	for each (Unit u in units)
	{
		if (u->getType() != UnitTypes::Zerg_Overlord && u->exists() && u->isTargetable()) {
			enemies.push_back(u);
		}
	}

	enemies.sort(comparePriorities);

	return enemies;
}

int Util::getPrioridade(BWAPI::Unit unit)
{
	if (unit->isAttacking()) {
		return 0;
	}
	else {
		return getPrioridade(unit->getType());
	}
}