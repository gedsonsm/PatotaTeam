#include "AAExample.h"
#include <Windows.h>

using namespace BWAPI;
using namespace Filter;

void AAExample::onStart()
{
	// Hello World!
	Broodwar->sendText("PatotaTeam");

	// Set the command optimization level so that common commands can be grouped
	// and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);

	// Retrieve you and your enemy's races. enemy() will just return the first enemy. AA: Not a problem with just 2.
	// If you wish to deal with multiple enemies then you must use enemies().
	if (Broodwar->enemy()) // First make sure there is an enemy AA: Make sure to check EVERYTHING.
		Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;

}

// For the hardcore ones, you can use Thread Pool closing it at onEnd.
void AAExample::onEnd(bool isWinner)
{
	//Broodwar << "onEnd:: " << isWinner << std::endl;
	// Called when the game ends
	util->GameOver = true;
	if (isWinner)
	{
		// AA: Do whatever you want, but make sure to not break anything.
		// Log your win here!
		Broodwar << "We won!!" << std::endl;
	}
	else {
		Broodwar << "Nah..." << std::endl;
	}
	// you don't need it if you end your threads without GameOver.
	Sleep(1000); // Enough time to end the threads.
}

void AAExample::onFrame()
{
	// Called once every game frame

	// Display the game frame rate as text in the upper left area of the screen
	//Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	//Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());

}

void AAExample::onSendText(std::string text)
{
	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());
	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!
}

void AAExample::onReceiveText(BWAPI::Player player, std::string text)
{ // AA: You won't need it because we don't have a team game, but it can change.
	// Parse the received text
	Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void AAExample::onPlayerLeft(BWAPI::Player player)
{
	// Interact verbally with the other players in the game by
	// announcing that the other player has left.
	//Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void AAExample::onNukeDetect(BWAPI::Position target)
{
	// AA: If you can make a nuke in 10 minutes, good for you.
	// Check if the target is a valid position
	if (target)
	{
		// if so, print the location of the nuclear strike target
		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else
	{
		// Otherwise, ask other players where the nuke is!
		Broodwar->sendText("Where's the nuke?");
	}
	// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void AAExample::onUnitDiscover(BWAPI::Unit unit)
{
}

void AAExample::onUnitEvade(BWAPI::Unit unit)
{
}

void AAExample::onUnitShow(BWAPI::Unit unit)
{
}

void AAExample::onUnitHide(BWAPI::Unit unit)
{
}

void AAExample::onUnitCreate(BWAPI::Unit unit)
{
	if (unit->getPlayer() == Broodwar->self())
	{
		if (unit->getType().isWorker()) 
		{
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

void AAExample::onUnitDestroy(BWAPI::Unit unit)
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

void AAExample::onUnitMorph(BWAPI::Unit unit)
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
		} else if (unit->getType() == UnitTypes::Zerg_Spawning_Pool) 
		{
			new SpawningPool(unit);
			util->qtdDrone--;
		}
		else if (unit->getType() == UnitTypes::Zerg_Egg)
		{
			util->qtdLarva--;
		}
		else if (unit->getType() == UnitTypes::Zerg_Overlord) 
		{
			new Overlord(unit);
			util->treinandoOverlord = false;
		}
	}
	


}

void AAExample::onUnitRenegade(BWAPI::Unit unit)
{
}

void AAExample::onSaveGame(std::string gameName)
{
}

void AAExample::onUnitComplete(BWAPI::Unit unit)
{
}
