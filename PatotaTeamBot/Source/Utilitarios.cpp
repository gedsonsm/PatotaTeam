#include "Utilitarios.h"

using namespace BWAPI;
using namespace Filter;

Utilitarios* Utilitarios::instanciaUtil;

Utilitarios::Utilitarios()
{
	GameOver = false;
	temPool = false;		 
	construindoPool = false;
	substituiDrone = false;
	scout = false;
	ataque = false;
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

Utilitarios* Utilitarios::getInstancia() //cria uma intancia da classe se ela não existe
{
	if (instanciaUtil == NULL) 
	{
		instanciaUtil = new Utilitarios();
	}
	return instanciaUtil;
}

PositionOrUnit Utilitarios::getAlvo()
{
	std::set<BWAPI::Unit> grupoInimigos = getInimigos(); //pega todas as unidades do inimigo
	BWAPI::PositionOrUnit algo = baseInimigo; 
	int priority = 0; //prioridade minima

	for each (BWAPI::Unit u in grupoInimigos)  //para cada inimigo do grupo obtido
	{
		if (u->exists() && u->isTargetable()) 
		{
			if (u->isAttacking()) // se o inimigo está atacando a prioridade é atacar ele
			{
				algo = u;
				break;
			}
			else
			{
				int unitPriority = getPrioridade(u->getType()); //seleciona o valor da prioridade 
				if (unitPriority > priority) //verifica se vai haver troca de prioridade
				{
					priority = unitPriority;
					algo = u;
				}
			}
		}
	}

	return algo;
}

Unit Utilitarios::getAlvoDefesa(Unit unit)
{
	std::list<BWAPI::Unit> list = getListaAlvos(); // pega todos os alvos que estam atacando
	std::list<BWAPI::Unit>::iterator it = list.begin();

	while (it != list.end())  // para cada alvo
	{
		if ((*it)->getDistance(unit) < 100)// se ele estiver perto, ataca!
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
		return 7;
	else if (unit == BWAPI::UnitTypes::Terran_SCV)
		return 6;
	else if (unit == BWAPI::UnitTypes::Terran_Bunker || unit == BWAPI::UnitTypes::Zerg_Drone || unit == BWAPI::UnitTypes::Protoss_Probe)
		return 5;
	else if (unit == BWAPI::UnitTypes::Terran_Barracks || unit == BWAPI::UnitTypes::Zerg_Spawning_Pool || unit == BWAPI::UnitTypes::Protoss_Pylon)
		return 4;
	else if (unit == BWAPI::UnitTypes::Terran_Command_Center || unit == BWAPI::UnitTypes::Zerg_Creep_Colony || unit == BWAPI::UnitTypes::Protoss_Nexus)
		return 3;
	else if (unit == BWAPI::UnitTypes::Terran_Supply_Depot || unit == BWAPI::UnitTypes::Protoss_Gateway)
		return 2;
	else
		return 1;
}

std::set<BWAPI::Unit> Utilitarios::getInimigos()
{
	const BWAPI::Unitset unidades = Broodwar->enemy()->getUnits(); //pega as unidades do enimigo
	std::set<BWAPI::Unit> grupoInimigos; 
	grupoInimigos.insert(unidades.begin(), unidades.end()); //insere todos os inimigos aqui
	return grupoInimigos;
}

Position Utilitarios::getBaseInimigo()
{
	TilePosition::list bases = Broodwar->getStartLocations(); //pega todas as localizações iniciais
	Position ret;
	if (bases.size() <= 2)
	{
		for each (TilePosition p in bases) //para cada base
		{
			if (p != Broodwar->self()->getStartLocation()) // se a localização não for a minha essa localização é a base inimiga
			{
				ret = (Position)p;
			}
		}
	}
	//else
	//{
	//	ret = (Position)Broodwar->self()->getStartLocation();
	//}
	return ret;

}

bool compararPrioridade(Unit& u1, Unit& u2) 
{
	bool ret = false;
	if (util->getPrioridade(u1) > util->getPrioridade(u2))
	{
		ret = true;
	}
	return ret;
}

std::list<BWAPI::Unit> Utilitarios::getListaAlvos() 
{
	const BWAPI::Unitset unidades = Broodwar->enemy()->getUnits(); //pega as unidades do enimigo
	std::list<Unit> grupoInimigos;
	for each (Unit u in unidades) // para cada unidade
	{
		if (u->getType() != UnitTypes::Zerg_Overlord && u->exists() && u->isTargetable())
		{
			grupoInimigos.push_back(u);
		}
	}
	grupoInimigos.sort(compararPrioridade); // vai ordenar a lista de acordo com a prioridade

	return grupoInimigos;
}

int Utilitarios::getPrioridade(BWAPI::Unit unit)
{
	if (!unit->isAttacking()) //se não seja atacando
	{
		return getPrioridade(unit->getType());
	}
	return 8; // se estiver atacando a prioridade é maxima
}