#include "PatotaTeamBot.h"
#include <Windows.h>

using namespace BWAPI;
using namespace Filter;

void PatotaTeamBot::onStart()
{
	// Hello World!
	Broodwar->sendText("");

	// Set the command optimization level so that common commands can be grouped
	// and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2); 

	// Retrieve you and your enemy's races. enemy() will just return the first enemy. AA: Not a problem with just 2.
	// If you wish to deal with multiple enemies then you must use enemies().
	if (Broodwar->enemy()) // First make sure there is an enemy AA: Make sure to check EVERYTHING.
		Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;

	new GerenciadorAtaque();

}

// For the hardcore ones, you can use Thread Pool closing it at onEnd.
void PatotaTeamBot::onEnd(bool isWinner)
{
	//Broodwar << "onEnd:: " << isWinner << std::endl;
	// Called when the game ends
	util->GameOver = true;
	if (isWinner)
	{
		// AA: Do whatever you want, but make sure to not break anything.
		// Log your win here!
		Broodwar << "We won!! :)" << std::endl;
	}
	else {
		Broodwar << "ohh :'(" << std::endl;
	}
	// you don't need it if you end your threads without GameOver.
	Sleep(1000); // Enough time to end the threads.
}

void PatotaTeamBot::onFrame()
{
	// Called once every game frame

	// Display the game frame rate as text in the upper left area of the screen
	//Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	//Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());
	Broodwar->drawTextScreen(100, 20, "PatotaTeam");

}

void PatotaTeamBot::onUnitCreate(BWAPI::Unit unit)
{
	if (unit->getPlayer() == Broodwar->self())
	{
		if (unit->getType().isWorker()) {
			new ZergDrone(unit);
			util->qtdDrone++;
		}
		else if (unit->getType() == UnitTypes::Zerg_Hatchery) 
		{
			new Hatchery(unit);
		}
		else if (unit->getType() == UnitTypes::Zerg_Zergling) 
		{
			new Zergling(unit);
			util->qtdZergling++;
		}
		else if (unit->getType() == UnitTypes::Zerg_Larva)
		{
			util->qtdLarva++;
		}
		else if (unit->getType() == UnitTypes::Zerg_Overlord)
		{
			new Overlord(unit);
		}
	}
}

void PatotaTeamBot::onUnitDestroy(BWAPI::Unit unit)
{
	if (unit->getPlayer() == Broodwar->self())
	{
		if (unit->getType().isWorker())
		{
			util->qtdDrone--;
		}
		else if (unit->getType() == UnitTypes::Zerg_Zergling) 
		{
			util->qtdZergling--;
		}
		else if (unit->getType() == UnitTypes::Zerg_Spawning_Pool) 
		{
			util->temPool = false;
			util->construindoPool = false;
		}
		else if (unit->getType() == UnitTypes::Zerg_Larva) 
		{
			util->qtdLarva--;
		}
	}
}

void PatotaTeamBot::onUnitMorph(BWAPI::Unit unit)
{
	if (unit->getPlayer() == Broodwar->self())
	{
		if (unit->getType().isWorker())
		{
			new ZergDrone(unit);
			util->qtdDrone++;
		}
		else if (unit->getType() == UnitTypes::Zerg_Zergling)
		{
			new Zergling(unit);
			util->qtdZergling++;
		} 
		else if (unit->getType() == UnitTypes::Zerg_Spawning_Pool) 
		{
			new SpawningPool(unit);
			util->qtdDrone--; //Usa um drone para morphing
		}
		else if (unit->getType() == UnitTypes::Zerg_Egg)
		{
			util->qtdLarva--;
		}
		else if (unit->getType() == UnitTypes::Zerg_Overlord) 
		{
			new Overlord(unit);
		}
	}
}