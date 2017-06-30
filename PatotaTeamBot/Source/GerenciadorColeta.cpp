#include "GerenciadorColeta.h"

using namespace BWAPI;
using namespace Filter;

GerenciadorColeta* GerenciadorColeta::instanciaColeta;

bool GerenciadorColeta::CompareDistance::operator() (const BWAPI::Unit & p_lhs, const BWAPI::Unit & p_rhs)
{
	const Position p = (Position)Broodwar->self()->getStartLocation();
	const size_t lhsDistance = p_lhs->getDistance(p);
	const size_t rhsDistance = p_rhs->getDistance(p);

	if (lhsDistance == rhsDistance)
	{
		return (p_lhs->getPosition() < p_rhs->getPosition()); 
	}

	return (lhsDistance < rhsDistance);
}


GerenciadorColeta::GerenciadorColeta()
{
	mineralList = getMineralFields();
	mineralQueueCount = new int[mineralList.size()];
	for (u_int i = 0; i < mineralList.size(); i++) {
		mineralQueueCount[i] = 0;
	}
}


GerenciadorColeta::~GerenciadorColeta()
{
}

GerenciadorColeta* GerenciadorColeta::getInstancia() {
	if (instanciaColeta == NULL) {
		instanciaColeta = new GerenciadorColeta();
	}
	return instanciaColeta;
}

BWAPI::Unit GerenciadorColeta::getMineralField(BWAPI::Unit unit) {
	BWAPI::Unit best_m = nullptr;
	int best_time = 999999;
	int i = 0;
	for each (BWAPI::Unit m in mineralList) {
		if (!m->exists()) {
			i++;
			continue;
		}
		int d_u_m = unit->getDistance(m->getPosition());
		int time = mineralQueueCount[i] * GATHER_TIME;
		int d_m_b = m->getDistance((Position)Broodwar->self()->getStartLocation());
		int totalTime = d_u_m + time + d_m_b;

		if (totalTime < best_time) {
			best_m = m;
			best_time = totalTime;
			i++;
		}
		else {
			break;
		}
	}
	mineralQueueCount[i - 1]++;
	return best_m;
}

void GerenciadorColeta::leaveQueue(BWAPI::Unit unit) {
	int i = 0;
	if (unit != nullptr) {
		for each (BWAPI::Unit m in mineralList) {
			if (unit == m) {
				mineralQueueCount[i]--;
				if (mineralQueueCount[i] < 0) {
					mineralQueueCount[i] = 0;
				}
				break;
			}
			i++;
		}
	}
}

MineralQueue GerenciadorColeta::getMineralFields() {
	const BWAPI::Unitset temp = Broodwar->getMinerals();
	MineralQueue minerals;
	minerals.insert(temp.begin(), temp.end());
	return minerals;
}
void GerenciadorColeta::insereNovosMinerais() 
{
	mineralList = getMineralFields();
	mineralQueueCount = new int[mineralList.size()];
	for (u_int i = 0; i < mineralList.size(); i++) 
	{
		//Broodwar->drawTextScreen(100, 50, "JAzidas: %d", mineralQueueCount[0]);
		mineralQueueCount[i] = 0;
	}
	
}

void GerenciadorColeta::printa()
{
	int i = 3;
	int j = 0;
	for each (BWAPI::Unit m in mineralList)
	{
		//Broodwar->drawTextScreen(i, 60, "%d",j);
		//printf("---%d/n",mineralList.size());
		i += 50;
		j++;
	}
}