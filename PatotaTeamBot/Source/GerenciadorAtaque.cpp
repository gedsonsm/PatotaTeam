#include "GerenciadorAtaque.h"


GerenciadorAtaque::GerenciadorAtaque()
{
	CreateThread(NULL, 0, GerenciadorAtaque::run, (LPVOID)this, 0, NULL);
}


GerenciadorAtaque::~GerenciadorAtaque()
{
}

bool GerenciadorAtaque::listaSelecionavel(std::list<BWAPI::Unit> list) 
{
	std::list<BWAPI::Unit>::iterator it = list.begin();
	while (it != list.end())
	{
		if ((*it)->isTargetable()) 
		{
			return true;
		}
		it++;
	}
	return false;
}

int GerenciadorAtaque::mixAtaqueZerglings(Unit u)
{
	UnitType unit = u->getType();
	if (unit == BWAPI::UnitTypes::Terran_SCV || unit == BWAPI::UnitTypes::Zerg_Drone || unit == BWAPI::UnitTypes::Protoss_Probe || unit == UnitTypes::Zerg_Larva)
	{
		return 5;
	}
	else
	{
		return 0;
	}
}

DWORD WINAPI GerenciadorAtaque::run(LPVOID param){
	GerenciadorAtaque* squad = static_cast<GerenciadorAtaque*>(param);
	DWORD dwWaitResult;
	Player self = BWAPI::Broodwar->self();
	PositionOrUnit alvo;
	PositionOrUnit ultimoAlvo = nullptr;
	std::list<BWAPI::Unit> list;
	bool andando = false;

	while (true){
		dwWaitResult = WaitForSingleObject(
			util->ghMutex,    // handle to mutex
			100);  // time-out interval

		//// If end game, or if it exists (remember to always check)
		if (util->GameOver)  {
			ReleaseMutex(util->ghMutex);
			return 0; // end thread
		} // end thread
		if (BWAPI::Broodwar->enemy()->getRace() != BWAPI::Races::Enum::Zerg)
		{
			if (!util->ataque && util->qtdZergling <= MIN_ATACANTES)
			{
				ReleaseMutex(util->ghMutex);
				Sleep(500);
				continue;
			}
		}
		else
		{
			if (!util->ataque && util->qtdZergling <= (MIN_ATACANTES - 4))
			{
				ReleaseMutex(util->ghMutex);
				Sleep(500);
				continue;
			}
		}
		if (dwWaitResult == WAIT_OBJECT_0 || dwWaitResult == WAIT_ABANDONED) //RAII
		{
			Unitset::iterator it = util->grupoAtaque.begin();
			while (it != util->grupoAtaque.end()) // limpa o grupo de atacantes das unidads que já morreram
			{
				if (!(*it)->exists()) 
				{
					it = util->grupoAtaque.erase(it);
				}
				else {
					it++;
				}
			}

			if (util->ataque && util->grupoAtaque.size() <= MIN_ATACANTES_PARAR_ATAQUE) // fica no aguardo ate ter um numero pra ataque
			{
				util->grupoAtaque.move((Position)self->getStartLocation());
				util->ataque = false;
			} 
			else 
			{
				util->ataque = true; // o ataque começa
				list = util->getListaAlvos(); //lega a lista de alvos
				if (list.empty()) // se a lista estiver vazia, é pq a base nao é a inimiga
				{
					if (!andando && util->baseInimigo != (Position)self->getStartLocation()) // move-se para a base inimiga
					{
						util->grupoAtaque.move(util->baseInimigo);
						andando = true;
					}
				}
				else if (list != squad->ultimaListaAtaque && squad->listaSelecionavel(list)) // verifica se a lista de alvos não é a msm
				{
					andando = false;
					squad->ultimaListaAtaque = list;

					if (util->grupoAtaque.size() < 10) //se o grupo de ataque for pequeno
					{
						util->grupoAtaque.attack(*list.begin());
					}
					else //para um grupo grande
					{
						std::list<BWAPI::Unit>::iterator it3 = list.begin(); // inimigos
						Unitset::iterator it2 = util->grupoAtaque.begin(); 
						while (it3 != list.end() && it2 != util->grupoAtaque.end())
						{
							if ((*it3)->exists() && (*it3)->isTargetable()) 
							{
								int max = squad->mixAtaqueZerglings(*it3); //seleciona um numero minimo de zerglings para um tipo de inimigo
								if (max == 0) //se não for SC, Drone Probe ou Larva ataca com toda a força
								{
									Unitset temp2;
									temp2.insert(it2, util->grupoAtaque.end());
									temp2.attack(*it3);
									it3 = list.end();
								}
								else // se for, pega alguns zerglings e manda atacar
								{
									Unitset temp;
									for (int i = 0; i < max && it2 != util->grupoAtaque.end(); i++)
									{
										temp.insert(*it2);
										it2++;
									}
									temp.attack(*it3);
									it3++;
								}
							}
							else
							{
								it3++; // pega o prox
							}
						}
					}
				}
			}


			if (!ReleaseMutex(util->ghMutex))
			{
				// Handle error.
			}
		}
		Sleep(30);
	}
}
