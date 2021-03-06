/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// Behaviors.h: Implementation of certain reusable behaviors for the BT version of the Agario Game
/*=============================================================================*/
#ifndef ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
#define ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "EliteMath/EMath.h"
#include "EBehaviorTree.h"
#include "SteeringBehaviors.h"
#include "IExamInterface.h"
//#include "SteeringSetter.h"
//struct SteeringSetter;


// Behaviors
BehaviorState RunOutOfPurgeZone(Elite::Blackboard* pBlackboard);
BehaviorState ManageInventory(Elite::Blackboard* pBlackboard);
BehaviorState GoToUnsearchedHouse(Elite::Blackboard* pBlackboard);
BehaviorState GrabAndAddItems(Elite::Blackboard* pBlackboard);
BehaviorState SeekClosestItem(Elite::Blackboard* pBlackboard);
BehaviorState ChangeToEvade(Elite::Blackboard* pBlackboard);
BehaviorState ChangeToSeek(Elite::Blackboard* pBlackboard);
BehaviorState ChangeToExploreMode(Elite::Blackboard* pBlackboard);
BehaviorState ChangeExploreLocation(Elite::Blackboard* pBlackboard);
BehaviorState ChangeToWander(Elite::Blackboard* pBlackboard);
BehaviorState Heal(Elite::Blackboard* pBlackboard);
BehaviorState Eat(Elite::Blackboard* pBlackboard);
BehaviorState AimAtZombie(Elite::Blackboard* pBlackboard);
BehaviorState Shoot(Elite::Blackboard* pBlackboard);
BehaviorState Run(Elite::Blackboard* pBlackboard);
BehaviorState GoToFood(Elite::Blackboard* pBlackboard);
BehaviorState GoToGun(Elite::Blackboard* pBlackboard);
BehaviorState GoToMedkit(Elite::Blackboard* pBlackboard);
BehaviorState ChangeToFlee(Elite::Blackboard* pBlackboard);
BehaviorState GetOutOfBeingStuck(Elite::Blackboard* pBlackboard);
//bools
bool AreEnemiesSight(Elite::Blackboard* pBlackboard);
bool AreItemsInGrabRange(Elite::Blackboard* pBlackboard);
bool AreItemsInSight(Elite::Blackboard* pBlackboard);
bool IsInExploreArea(Elite::Blackboard* pBlackboard);
bool IsRoomEmpty(Elite::Blackboard* pBlackboard);
bool IsRoomNotEmpty(Elite::Blackboard* pBlackboard);
bool HasInventoryChanged(Elite::Blackboard* pBlackboard);
bool HaveGunAndAmmo(Elite::Blackboard* pBlackboard);
bool HasNotCheckedHouse(Elite::Blackboard* pBlackboard);
bool HasMedKit(Elite::Blackboard* pBlackboard);
bool HasNotCheckedHouse(Elite::Blackboard* pBlackboard);
bool IsEnergyLow(Elite::Blackboard* pBlackboard);
bool IsHealthLow(Elite::Blackboard* pBlackboard);
bool HasFood(Elite::Blackboard* pBlackboard);
bool TookDMG(Elite::Blackboard* pBlackboard);
bool TooManyEnemiesInSight(Elite::Blackboard* pBlackboard);
bool IsInventoryFull(Elite::Blackboard* pBlackboard);
bool IsInPurgeZone(Elite::Blackboard* pBlackboard);
bool InNeedOfGun(Elite::Blackboard* pBlackboard);
bool IsStuck(Elite::Blackboard* pBlackboard);

//bool CloseToNeededItem(Elite::Blackboard* pBlackboard);


//extras
void AdjustLength(float& a);



//-----------------------------------------------------------------
// Behaviors
//-----------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////////////////////////////
//Tests: to see wich is best option to pick// exploring first though////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////
//BOOLS/////////////////////////////////////////////
////////////////////////////////////////////////////
//This bool is part of the Generator 
//checking if the house is in the vector or no and see if it has been searched in or not
//0 means not checked and -1 means was empty when i found it, above 0 means how much things were in their

BehaviorState GetOutOfBeingStuck(Elite::Blackboard* pBlackboard)
{

	ISteeringBehavior** pBehavior = nullptr;
	Elite::Vector2** pTarget = nullptr;
	Elite::Vector2** exploreTarget = nullptr;
	IExamInterface* pInterface = nullptr;
	bool isGettingOut{};
	bool* isScared = nullptr;
	bool* isStuck = nullptr;
	Vector2 stuckPoint{};
	const float radius{ 5.f };
	pBlackboard->GetData("isScared", isScared);
	pBlackboard->GetData("IsStuck", isStuck);
	pBlackboard->GetData("isGettingOut", isGettingOut);//
	pBlackboard->GetData("stuckPos", stuckPoint);//
	auto dataAvailable = pBlackboard->GetData("SteeringBehavior", pBehavior) && pBlackboard->GetData("pTarget", pTarget) && pBlackboard->GetData("pExploreTarget", exploreTarget) && pBlackboard->GetData("pInterface", pInterface);



	if (isGettingOut && pInterface->Agent_GetInfo().Position.Distance(stuckPoint) < radius)
	{
		std::cout << "i am still GettingOut\n";
		return Success;
	}


	if (*isScared)
	{
		**pTarget *= -1;
		**pTarget = pInterface->NavMesh_GetClosestPathPoint(**pTarget);
	}
	else
	{
		Vector2 tmp = **exploreTarget;
		tmp *= -1;
		**pTarget = tmp;
		**pTarget = pInterface->NavMesh_GetClosestPathPoint(**pTarget);
	}

	isGettingOut = true;
	delete* pBehavior;
	*pBehavior = new Seek();

	std::cout << "GettingOut\n";
	return Success;
}




BehaviorState RunOutOfPurgeZone(Elite::Blackboard* pBlackboard)
{
	PurgeZoneInfo purgeZone{};
	ISteeringBehavior** pBehavior;
	Elite::Vector2** pTarget{};
	IExamInterface* pInterface = nullptr;
	pBlackboard->GetData("purgeZoneItIsIn", purgeZone);

	auto dataAvailable = pBlackboard->GetData("SteeringBehavior", pBehavior) && pBlackboard->GetData("pTarget", pTarget) && pBlackboard->GetData("pInterface", pInterface);

	AgentInfo agent = pInterface->Agent_GetInfo();



	float degrees{ atan2(purgeZone.Center.y, purgeZone.Center.x) };

	if (agent.Orientation > degrees)
	{
		std::cout << "above!\n";
	}
	else
	{
		std::cout << "down!\n";
	}


	return Success;
}

BehaviorState GoToGun(Elite::Blackboard* pBlackboard)
{
	std::vector<std::pair<ItemInfo, bool>> unwantedVecItems{};
	Elite::Vector2** pTarget{};
	IExamInterface* pInterface = nullptr;//DONT FORGET TO CHANGE SEEK CLOSEST FOOD
	pBlackboard->GetData("VectorUnwantedItems", unwantedVecItems);
	pBlackboard->GetData("pTarget", pTarget);
	pBlackboard->GetData("pInterface", pInterface);


	if (unwantedVecItems.size() == 0)
	{
		return Failure;
	}

	//std::min_element(unwantedVecItems.begin(), unwantedVecItems.end());
	auto agentInf = pInterface->Agent_GetInfo();


	std::vector<std::pair<Vector2, int>> tmpMedVec{};



	for (size_t i = 0; i < unwantedVecItems.size(); i++)
	{
		if (unwantedVecItems[i].first.Type == eItemType::MEDKIT)
		{
			tmpMedVec.push_back({ unwantedVecItems[i].first.Location, i });
		}
	}



	if (tmpMedVec.size() == 0)
	{
		return Failure;
	}
	else if (tmpMedVec.size() == 1)
	{
		unwantedVecItems[tmpMedVec.front().second].second = true;
		std::cout << "GO TO Gun!!\n";
		(**pTarget) = pInterface->NavMesh_GetClosestPathPoint(tmpMedVec.front().first);
		pBlackboard->ChangeData("VectorUnwantedItems", unwantedVecItems);
		return Success;
	}


	std::pair<Vector2, int> tmp = *std::min_element(tmpMedVec.begin(), tmpMedVec.end(), [&agentInf](std::pair<Vector2, int>& a, std::pair<Vector2, int>& b)
		{

			return (agentInf.Position.Distance(a.first) <= agentInf.Position.Distance(b.first));

		});

	unwantedVecItems[tmp.second].second = true;

	std::cout << "GO TO Gun!!\n";
	(**pTarget) = pInterface->NavMesh_GetClosestPathPoint(tmp.first);
	pBlackboard->ChangeData("VectorUnwantedItems", unwantedVecItems);
	return Success;
}
BehaviorState GoToMedkit(Elite::Blackboard* pBlackboard)
{
	std::vector<std::pair<ItemInfo, bool>> unwantedVecItems{};
	Elite::Vector2** pTarget{};
	IExamInterface* pInterface = nullptr;//DONT FORGET TO CHANGE SEEK CLOSEST FOOD
	pBlackboard->GetData("VectorUnwantedItems", unwantedVecItems);
	pBlackboard->GetData("pTarget", pTarget);
	pBlackboard->GetData("pInterface", pInterface);

	if (unwantedVecItems.size() == 0)
	{
		return Failure;
	}
	//std::min_element(unwantedVecItems.begin(), unwantedVecItems.end());
	auto agentInf = pInterface->Agent_GetInfo();

	std::vector<std::pair<Vector2, int>> tmpMedVec{};



	for (size_t i = 0; i < unwantedVecItems.size(); i++)
	{
		if (unwantedVecItems[i].first.Type == eItemType::MEDKIT)
		{
			tmpMedVec.push_back({ unwantedVecItems[i].first.Location, i });
		}
	}



	if (tmpMedVec.size() == 0)
	{
		return Failure;
	}
	else if (tmpMedVec.size() == 1)
	{
		unwantedVecItems[tmpMedVec.front().second].second = true;
		std::cout << "GO TO MEDKIT!!\n";
		(**pTarget) = pInterface->NavMesh_GetClosestPathPoint(tmpMedVec.front().first);
		pBlackboard->ChangeData("VectorUnwantedItems", unwantedVecItems);
		return Success;
	}


	std::pair<Vector2, int> tmp = *std::min_element(tmpMedVec.begin(), tmpMedVec.end(), [&agentInf](std::pair<Vector2, int>& a, std::pair<Vector2, int>& b)
		{

			return (agentInf.Position.Distance(a.first) <= agentInf.Position.Distance(b.first));

		});

	unwantedVecItems[tmp.second].second = true;

	std::cout << "GO TO MEDKIT!!\n";
	(**pTarget) = pInterface->NavMesh_GetClosestPathPoint(tmp.first);
	pBlackboard->ChangeData("VectorUnwantedItems", unwantedVecItems);
	return Success;

}
BehaviorState GoToFood(Elite::Blackboard* pBlackboard)
{
	std::vector<std::pair<ItemInfo, bool>> unwantedVecItems{};
	Elite::Vector2** pTarget{};
	IExamInterface* pInterface = nullptr;//DONT FORGET TO CHANGE SEEK CLOSEST FOOD
	pBlackboard->GetData("VectorUnwantedItems", unwantedVecItems);
	pBlackboard->GetData("pTarget", pTarget);
	pBlackboard->GetData("pInterface", pInterface);



	if (unwantedVecItems.size() == 0)
	{
		return Failure;
	}

	//std::min_element(unwantedVecItems.begin(), unwantedVecItems.end());
	auto agentInf = pInterface->Agent_GetInfo();


	std::vector<std::pair<Vector2, int>> tmpMedVec{};



	for (size_t i = 0; i < unwantedVecItems.size(); i++)
	{
		if (unwantedVecItems[i].first.Type == eItemType::FOOD)
		{
			tmpMedVec.push_back({ unwantedVecItems[i].first.Location, i });
		}
	}



	if (tmpMedVec.size() == 0)
	{
		return Failure;
	}
	else if (tmpMedVec.size() == 1)
	{
		unwantedVecItems[tmpMedVec.front().second].second = true;
		std::cout << "GO TO Food!!\n";
		(**pTarget) = pInterface->NavMesh_GetClosestPathPoint(tmpMedVec.front().first);
		pBlackboard->ChangeData("VectorUnwantedItems", unwantedVecItems);
		return Success;
	}


	std::pair<Vector2, int> tmp = *std::min_element(tmpMedVec.begin(), tmpMedVec.end(), [&agentInf](std::pair<Vector2, int>& a, std::pair<Vector2, int>& b)
		{

			return (agentInf.Position.Distance(a.first) <= agentInf.Position.Distance(b.first));

		});

	unwantedVecItems[tmp.second].second = true;

	std::cout << "GO TO Food!!\n";
	(**pTarget) = pInterface->NavMesh_GetClosestPathPoint(tmp.first);
	pBlackboard->ChangeData("VectorUnwantedItems", unwantedVecItems);
	return Success;
}

bool TookDMG(Elite::Blackboard* pBlackboard)
{
	float prevFrameHp{ 0.f };
	IExamInterface* pInterface = nullptr;
	bool* isScared{ false };
	bool** canRun = nullptr;
	pBlackboard->GetData("prevFrameHp", prevFrameHp);
	pBlackboard->GetData("pInterface", pInterface);
	pBlackboard->GetData("isScared", isScared);
	pBlackboard->GetData("canRun", canRun);

	if (pInterface->Agent_GetInfo().Stamina < 1)
	{
		**canRun = false;
		return false;
	}


	return (pInterface->Agent_GetInfo().Health < prevFrameHp && !*isScared);
}
bool IsHealthLow(Elite::Blackboard* pBlackboard)
{
	/*std::unordered_map<std::string, int> inventory{};
	pBlackboard->GetData("Inventory", inventory);*/
	const float maxOkayHP{ 7.5f };
	IExamInterface* pInterface = nullptr;

	pBlackboard->GetData("pInterface", pInterface);


	return pInterface->Agent_GetInfo().Health < maxOkayHP;
}

bool HasMedKit(Elite::Blackboard* pBlackboard)
{
	std::unordered_map<std::string, int> inventory{};
	pBlackboard->GetData("Inventory", inventory);

	return inventory["Medkit"] > 0;
}


bool IsEnergyLow(Elite::Blackboard* pBlackboard)
{
	/*std::unordered_map<std::string, int> inventory{};
	pBlackboard->GetData("Inventory", inventory);*/
	const float maxOkayEnergy{ 5.5f };
	IExamInterface* pInterface = nullptr;

	pBlackboard->GetData("pInterface", pInterface);


	return pInterface->Agent_GetInfo().Energy < maxOkayEnergy;
}

bool HasFood(Elite::Blackboard* pBlackboard)
{
	std::unordered_map<std::string, int> inventory{};
	pBlackboard->GetData("Inventory", inventory);

	return inventory["Food"] > 0;
}

bool HasNotCheckedHouse(Elite::Blackboard* pBlackboard)
{
	std::vector<std::pair<Vector2, int>> houses;
	pBlackboard->GetData("housesKnownLocations", houses);
	IExamInterface* pInterface = nullptr;

	pBlackboard->GetData("pInterface", pInterface);


	HouseInfo hi = {};
	for (int i = 0;; ++i)
	{
		if (pInterface->Fov_GetHouseByIndex(i, hi))
		{
			int counter{ 0 };
			for (std::pair<Vector2, int>& a : houses)
			{
				if (hi.Center == a.first)
				{
					pBlackboard->ChangeData("HouseIndex", counter);
					if (a.second == 0)
					{
						return true;
					}
					else if (a.second == -1)
					{
						return false;
					}
					else
					{
						return true;
					}

				}
				counter++;
			}
			houses.push_back({ hi.Center,0 });
			pBlackboard->ChangeData("housesKnownLocations", houses);
			continue;
		}

		break;
	}


	return false;
}
//checksIfBotHasAmmoAndaGun
bool HaveGunAndAmmo(Elite::Blackboard* pBlackboard)
{
	std::unordered_map<std::string, int> inventory{};
	IExamInterface* pInterface = nullptr;
	bool* isAiming = nullptr;
	pBlackboard->GetData("Inventory", inventory);
	pBlackboard->GetData("pInterface", pInterface);
	pBlackboard->GetData("IsAiming", isAiming);


	if (*isAiming)
	{
		return true;
	}
	else if (inventory["Pistol"] < 1)
	{
		return false;
	}

	std::cout << "CheckAmmo!!!\n";

	ItemInfo tmpItem{};
	for (size_t i = 0; i < pInterface->Inventory_GetCapacity(); i++)
	{
		if (pInterface->Inventory_GetItem(i, tmpItem))
		{
			if (tmpItem.Type == eItemType::PISTOL)
			{
				if (pInterface->Weapon_GetAmmo(tmpItem) < 1)
				{
					pInterface->Inventory_RemoveItem(i);
				}
				else
				{
					return true;
				}

			}
		}
	}

	return false;
}

bool InNeedOfGun(Elite::Blackboard* pBlackboard)
{
	std::vector<std::pair<ItemInfo, bool>> unwantedVecItems{};
	std::unordered_map<std::string, int> inventory{};
	pBlackboard->GetData("Inventory", inventory);
	pBlackboard->GetData("VectorUnwantedItems", unwantedVecItems);



	if (unwantedVecItems.size() < 1)
	{
		return false;
	}

	for (std::pair<ItemInfo, bool> a : unwantedVecItems)
	{
		if (a.first.Type == eItemType::PISTOL)
		{

			return inventory["Pistol"] < 1;
		}
	}

	return false;
}





//
bool HasInventoryChanged(Elite::Blackboard* pBlackboard)
{
	bool hasChanged{ false };
	pBlackboard->GetData("InventoryChanged", hasChanged);
	return hasChanged;
}

bool IsRoomNotEmpty(Elite::Blackboard* pBlackboard)
{
	const float radius{ 5.5f };
	std::vector<std::pair<Vector2, int>> houses;
	IExamInterface* pInterface = nullptr;
	int index{ 0 };
	pBlackboard->GetData("HouseIndex", index);
	pBlackboard->GetData("housesKnownLocations", houses);
	pBlackboard->GetData("pInterface", pInterface);

	AgentInfo agent = pInterface->Agent_GetInfo();

	if (agent.Position.Distance(houses[index].first) < radius && !AreItemsInSight(pBlackboard))
	{
		if (houses[index].second == 0)
		{
			houses[index].second = -1;
			pBlackboard->ChangeData("housesKnownLocations", houses);
			//return false;
		}
		return false;
	}



	return true;
}
bool IsRoomEmpty(Elite::Blackboard* pBlackboard)
{
	const float radius{ 2.5f };
	std::vector<std::pair<Vector2, int>> houses;
	IExamInterface* pInterface = nullptr;
	int index{ 0 };
	pBlackboard->GetData("HouseIndex", index);
	pBlackboard->GetData("housesKnownLocations", houses);
	pBlackboard->GetData("pInterface", pInterface);

	AgentInfo agent = pInterface->Agent_GetInfo();

	if (agent.Position.Distance(houses[index].first) < radius && !AreItemsInSight(pBlackboard))
	{
		if (houses[index].second == 0)
		{
			houses[index].second = -1;
			pBlackboard->ChangeData("housesKnownLocations", houses);
			//return false;
		}
		return true;
	}



	return false;
}


//items
bool IsInExploreArea(Elite::Blackboard* pBlackboard)
{
	const float maxRange{ 160.f };
	const float radius{ 10.f };
	IExamInterface* pInterface = nullptr;
	Elite::Vector2** exploreTarget = nullptr;
	bool* isScared = nullptr;
	pBlackboard->GetData("pInterface", pInterface);
	pBlackboard->GetData("pExploreTarget", exploreTarget);
	AgentInfo agent = pInterface->Agent_GetInfo();
	pBlackboard->GetData("isScared", isScared);


	if (agent.Position.Distance({0, 0}) > maxRange)
	{
		return true;
	}

	*isScared = false;
	return agent.Position.Distance(**exploreTarget) <= radius;


	return false;
}
bool AreItemsInSight(Elite::Blackboard* pBlackboard)
{
	std::vector<EntityInfo>* pVecFOVEItems;
	pBlackboard->GetData("VectorFOVItems", pVecFOVEItems);
	std::vector<std::pair<ItemInfo, bool>> unwantedVecItems{};
	pBlackboard->GetData("VectorUnwantedItems", unwantedVecItems);


	if (pVecFOVEItems->size() == 0)
	{
		return false;
	}


	pVecFOVEItems->erase(std::remove_if(pVecFOVEItems->begin(), pVecFOVEItems->end(), [unwantedVecItems](auto b) {

		for (std::pair<ItemInfo, bool> a : unwantedVecItems)
		{
			if (!a.second)
			{
				return b.Location == a.first.Location;
			}

		}


		; }), pVecFOVEItems->end());




	pBlackboard->ChangeData("VectorFOVItems", pVecFOVEItems);




	//m_pInterface

	return true;
}

bool AreItemsInGrabRange(Elite::Blackboard* pBlackboard)
{
	IExamInterface* pInterface = nullptr;
	std::vector<EntityInfo>* pVecFOVEItems = nullptr;
	pBlackboard->GetData("VectorFOVItems", pVecFOVEItems);
	pBlackboard->GetData("pInterface", pInterface);
	AgentInfo agent = pInterface->Agent_GetInfo();



	for (auto a : *pVecFOVEItems)
	{
		if (agent.GrabRange >= agent.Position.Distance(a.Location))
		{
			
			return true;
		}
	}

	return false;

}
//enemies
bool AreEnemiesSight(Elite::Blackboard* pBlackboard)
{
	bool* pIsAiming = nullptr;
	std::vector<Vector2>* pVecFOVEnemiesHash = nullptr;
	Elite::Vector2** pTarget{};
	IExamInterface* pInterface = nullptr;
	bool* isScared = nullptr;
	bool** canRun = nullptr;
	pBlackboard->GetData("VectorFOVEnemiesPos", pVecFOVEnemiesHash);
	auto dataAvailable = pBlackboard->GetData("pInterface", pInterface) && pBlackboard->GetData("pTarget", pTarget);
	pBlackboard->GetData("isScared", isScared);
	pBlackboard->GetData("canRun", canRun);
	pBlackboard->GetData("IsAiming", pIsAiming);

	if (!dataAvailable)
	{
		return false;
	}
	else if (*isScared)
	{
		const float halfStamina{ 5.f };
		if (pInterface->Agent_GetInfo().Stamina < halfStamina)
		{
			*isScared = false;
			//pBlackboard->ChangeData("isScared", false);
			**canRun = false;
		}
		return true;
	}
	else if (pVecFOVEnemiesHash->size() == 0)
	{
		*pIsAiming = false;
		//**canRun = false;
		return false;
	}
	else if (pVecFOVEnemiesHash->size() == 1)
	{

		**pTarget = pVecFOVEnemiesHash->front();
		return true;
	}

	auto agentInf = pInterface->Agent_GetInfo();

	(**pTarget) = *std::min_element(pVecFOVEnemiesHash->begin(), pVecFOVEnemiesHash->end(), [&agentInf](const Vector2& a, const Vector2& b) {

		return (agentInf.Position.Distance(a)) <= (agentInf.Position.Distance(b))
			; });

	return true;
}


////////////////////////////////////////////////////
//BEHAVIORS/////////////////////////////////////////
////////////////////////////////////////////////////



//wander
BehaviorState ChangeToWander(Elite::Blackboard* pBlackboard)
{
	//SteeringSetter* pSter;
	ISteeringBehavior** pBehavior;
	//auto dataAvailable = pBlackboard->GetData("SteeringSetter", pSter);
	pBlackboard->ChangeData("isInExplorerMode", false);
	auto dataAvailable = pBlackboard->GetData("SteeringBehavior", pBehavior);

	if (!dataAvailable)
		return Failure;
	delete* pBehavior;
	*pBehavior = new Wander();
	//pSter->SetToWander();

	return Success;
}

BehaviorState ChangeExploreLocation(Elite::Blackboard* pBlackboard)
{
	bool isClockWise{};
	const float length{ 130.f };
	pBlackboard->GetData("isClockWise", isClockWise);
	ISteeringBehavior** pBehavior = nullptr;
	Elite::Vector2** pTarget = nullptr;
	Elite::Vector2** exploreTarget = nullptr;
	IExamInterface* pInterface = nullptr;
	pBlackboard->GetData("pInterface", pInterface);
	auto dataAvailable = pBlackboard->GetData("SteeringBehavior", pBehavior) && pBlackboard->GetData("pTarget", pTarget) && pBlackboard->GetData("pExploreTarget", exploreTarget);
	if (!dataAvailable)
	{
		return Failure;
	}



	if (isClockWise)
	{
		if ((*exploreTarget)->x == (*exploreTarget)->y)
		{

			if ((*exploreTarget)->y < 0)
			{
				(*exploreTarget)->y = length;
			}
			else
			{
				(*exploreTarget)->y = -length;
			}

		}
		else if ((*exploreTarget)->x < 0)
		{
			(*exploreTarget)->x = length;
		}
		else
		{
			(*exploreTarget)->x = -length;
		}

	}
	else
	{

		if ((*exploreTarget)->x == (*exploreTarget)->y)
		{
			if ((*exploreTarget)->x < 0)
			{
				(*exploreTarget)->x = length;
			}
			else
			{
				(*exploreTarget)->x = -length;
			}

		}
		else if ((*exploreTarget)->y < 0)
		{
			(*exploreTarget)->y = length;
		}
		else
		{
			(*exploreTarget)->y = -length;
		}
	}
	AdjustLength((*exploreTarget)->x);
	AdjustLength((*exploreTarget)->y);


	pBlackboard->ChangeData("isInExplorerMode", true);
	//*pTarget = *exploreTarget;
	//delete* pBehavior;
	//*pBehavior = new Seek();
	**pTarget = pInterface->NavMesh_GetClosestPathPoint((**exploreTarget));
	delete* pBehavior;
	*pBehavior = new Seek();



	return Success;
}
BehaviorState ChangeToExploreMode(Elite::Blackboard* pBlackboard)
{
	const float maxDistance{ 5.f };
	ISteeringBehavior** pBehavior = nullptr;
	Elite::Vector2** pTarget = nullptr;
	Elite::Vector2** exploreTarget = nullptr;
	IExamInterface* pInterface = nullptr;
	bool isInExplorerMode{ false };
	pBlackboard->GetData("isInExplorerMode", isInExplorerMode);
	bool ssSearchingHouse{ false };
	pBlackboard->GetData("IsSearchingHouse", ssSearchingHouse);
	auto dataAvailable = pBlackboard->GetData("SteeringBehavior", pBehavior) && pBlackboard->GetData("pTarget", pTarget) && pBlackboard->GetData("pExploreTarget", exploreTarget) && pBlackboard->GetData("pInterface", pInterface);


	pBlackboard->ChangeData("isInExplorerMode", true);

	if (!dataAvailable)
	{
		return Failure;
	}
	else if (ssSearchingHouse)
	{
		return Failure;
	}


	std::cout << "Exploring!!!!\n";/////////////



	**pTarget = pInterface->NavMesh_GetClosestPathPoint((**exploreTarget));
	delete* pBehavior;
	*pBehavior = new Seek();
	//pSter->SetToWander();

	return Success;
}
//Seek
BehaviorState ChangeToSeek(Elite::Blackboard* pBlackboard)
{
	//SteeringSetter* pSter;
	ISteeringBehavior** pBehavior;
	Elite::Vector2** pTarget{};
	Elite::Vector2* target{};

	pBlackboard->ChangeData("isInExplorerMode", false);
	//auto dataAvailable = pBlackboard->GetData("SteeringSetter", pSter);
	auto dataAvailable = pBlackboard->GetData("SteeringBehavior", pBehavior) && pBlackboard->GetData("Target", target) && pBlackboard->GetData("pTarget", pTarget);

	if (!dataAvailable)
		return Failure;

	//*pTarget = pInterface->NavMesh_GetClosestPathPoint(target);
	*pTarget = target;
	delete* pBehavior;
	*pBehavior = new Seek();
	//pSter->SetToWander();

	return Success;
}
///flee
BehaviorState ChangeToFlee(Elite::Blackboard* pBlackboard)
{
	ISteeringBehavior** pBehavior;
	Elite::Vector2 target{};
	Elite::Vector2** pTarget{};

	std::vector<Vector2>* pVecFOVEnemiesHash = nullptr;
	pBlackboard->ChangeData("isInExplorerMode", false);
	auto dataAvailable = pBlackboard->GetData("SteeringBehavior", pBehavior) /*&& pBlackboard->GetData("Target", target)*/;
	pBlackboard->GetData("VectorFOVEnemiesPos", pVecFOVEnemiesHash);
	pBlackboard->GetData("pTarget", pTarget);
	IExamInterface* pInterface = nullptr;
	pBlackboard->GetData("pInterface", pInterface);



	auto agentInf = pInterface->Agent_GetInfo();

	if (!dataAvailable)
	{
		return Failure;
	}




	auto tmp = std::min_element(pVecFOVEnemiesHash->begin(), pVecFOVEnemiesHash->end(), [&agentInf](const Vector2& a, const Vector2& b) {

		return (agentInf.Position.Distance(a)) < (agentInf.Position.Distance(b))
			; });





	if (tmp != pVecFOVEnemiesHash->end())
	{
		**pTarget = *tmp;
	}


	//**pTarget = fleePoint;
	delete* pBehavior;
	*pBehavior = new Evade();
	//finding the hashes location and look who is the closest

	std::cout << "Flee!!!!\n";


	return Success;


}

//Evade
BehaviorState ChangeToEvade(Elite::Blackboard* pBlackboard)
{
	const float maxTime{ 1.5f };
	ISteeringBehavior** pBehavior = nullptr;
	Elite::Vector2** pTarget = nullptr;
	IExamInterface* pInterface = nullptr;
	bool* isScared{ false };
	bool** canRun = nullptr;
	Elite::Vector2** exploreTarget = nullptr;
	float* scaredTimer = nullptr;
	pBlackboard->GetData("isScared", isScared);
	pBlackboard->GetData("canRun", canRun);
	pBlackboard->GetData("scaredTimer", scaredTimer);
	bool ssSearchingHouse{ false };
	pBlackboard->GetData("IsSearchingHouse", ssSearchingHouse);
	auto dataAvailable = pBlackboard->GetData("SteeringBehavior", pBehavior) && pBlackboard->GetData("pTarget", pTarget) && pBlackboard->GetData("pInterface", pInterface) && pBlackboard->GetData("pExploreTarget", exploreTarget);

	if (!dataAvailable)
	{
		return Failure;
	}
	else if (*isScared && *scaredTimer > maxTime)
	{
		*isScared = false;
		return Success;
	}
	else if (!isScared)
	{
		**canRun = true;
	}
	*isScared = new bool(true);
	std::cout << "Evading!!!!\n";

	float degrees{ atan2((*pTarget)->y, (*pTarget)->x) };
	//const float angle{ ToRadians(45.f) };
	const float space{ 1.5f };




	//** pTarget = ((pInterface->Agent_GetInfo().Position) + (**exploreTarget)) - **pTarget;
	//**pTarget = ((pInterface->Agent_GetInfo().Position) + (**exploreTarget)/2) - **pTarget;
	**pTarget = ((pInterface->Agent_GetInfo().Position) + (**exploreTarget) * 0.5f) - **pTarget;
	//**pTarget = pInterface->NavMesh_GetClosestPathPoint((**pTarget));


	**pTarget = pInterface->NavMesh_GetClosestPathPoint((**pTarget));



	delete* pBehavior;
	*pBehavior = new Seek();
	//finding the hashes location and look who is the closest




	return Success;


}

//SeekClosestItems
BehaviorState SeekClosestItem(Elite::Blackboard* pBlackboard)
{
	ISteeringBehavior** pBehavior;
	Elite::Vector2** pTarget{};
	std::vector<EntityInfo>* pVecFOVEItems;
	IExamInterface* pInterface = nullptr;
	pBlackboard->ChangeData("isInExplorerMode", false);
	pBlackboard->GetData("VectorFOVItems", pVecFOVEItems);

	auto dataAvailable = pBlackboard->GetData("SteeringBehavior", pBehavior) && pBlackboard->GetData("pTarget", pTarget) && pBlackboard->GetData("pInterface", pInterface);

	std::cout << "SeekingCLosestITEM!!\n";
	if (!dataAvailable)
	{
		return Failure;
	}
	else if (pVecFOVEItems->size() == 0)
	{
		return Failure;
	}
	else if (pVecFOVEItems->size() == 1)
	{

		**pTarget = pVecFOVEItems->front().Location;
		delete* pBehavior;
		*pBehavior = new Seek();
		return Success;
	}

	auto agentInf = pInterface->Agent_GetInfo();

	EntityInfo tmp;
	(tmp) = *std::min_element(pVecFOVEItems->begin(), pVecFOVEItems->end(), [agentInf](const EntityInfo& a, const EntityInfo& b) {

		return (agentInf.Position.Distance(a.Location)) < (agentInf.Position.Distance(b.Location))
			; });

	**pTarget = tmp.Location;

	delete* pBehavior;
	*pBehavior = new Seek();

	return Success;

}
//GrabItems
BehaviorState GrabAndAddItems(Elite::Blackboard* pBlackboard)
{
	std::unordered_map<std::string, int> inventory{};
	const float houseRad{ 20.f };
	IExamInterface* pInterface = nullptr;
	std::vector<EntityInfo>* pVecFOVEItems;
	std::vector<std::pair<Vector2, int>> houses;
	std::vector<std::pair<ItemInfo, bool>> unwantedVecItems{};
	int houseIndex{ 0 };
	pBlackboard->GetData("HouseIndex", houseIndex);
	pBlackboard->GetData("Inventory", inventory);
	pBlackboard->GetData("VectorUnwantedItems", unwantedVecItems);
	//pBlackboard->GetData("housesKnownLocations", houses);
	const int maxDuplicates{ 2 };//so that there be no more then 2 duplicates

	auto dataAvailable = pBlackboard->GetData("VectorFOVItems", pVecFOVEItems) && pBlackboard->GetData("pInterface", pInterface) && pBlackboard->GetData("housesKnownLocations", houses);

	if (!dataAvailable)
	{
		return Failure;
	}


	auto agentInf = pInterface->Agent_GetInfo();

	std::cout << "GrabAndADDItems!!\n";
	for (EntityInfo a : *pVecFOVEItems)
	{
		ItemInfo tmpInfo{};

		if (pInterface->Item_Grab(a, tmpInfo))
		{
			
			for (const std::pair<ItemInfo, bool>& ib : unwantedVecItems)
			{
				if (!ib.second && ib.first.Location == tmpInfo.Location)
				{
					return Success;
				}
			}
			ItemInfo tmpItem{};
			for (size_t i = 0; i < pInterface->Inventory_GetCapacity(); i++)
			{
				if (!pInterface->Inventory_GetItem(i, tmpItem))
				{
			


					switch (tmpInfo.Type)
					{
					case eItemType::PISTOL:
						if (inventory["Pistol"] >= maxDuplicates)
						{
							std::cout << "Tomany Pistal\n";
							unwantedVecItems.push_back({ tmpInfo,false });
							pBlackboard->ChangeData("VectorUnwantedItems", unwantedVecItems);
							return Success;
						}
						//inventory["Pistol"]++;

						break;
					case eItemType::MEDKIT:
						if (inventory["Medkit"] >= maxDuplicates)
						{
							std::cout << "Tomany Medkit\n";
							unwantedVecItems.push_back({ tmpInfo,false });
							pBlackboard->ChangeData("VectorUnwantedItems", unwantedVecItems);
							return Success;
						}
						//	inventory["Medkit"]++;
							//inventory["Medkit"]++;
						break;
					case eItemType::FOOD:
						if (inventory["Food"] >= maxDuplicates)
						{
							std::cout << "Tomany Food\n";
							unwantedVecItems.push_back({ tmpInfo,false });
							pBlackboard->ChangeData("VectorUnwantedItems", unwantedVecItems);
							return Success;
						}
						//inventory["Food"]++;
						break;
					case eItemType::GARBAGE:
						pInterface->Item_Destroy(a);
						return Success;
						//inventory["Garbage"]++;
						break;
					}
					//pInterface->Inventory_AddItem(i, tmpInfo);
					unwantedVecItems.erase(std::remove_if(unwantedVecItems.begin(), unwantedVecItems.end(), [tmpInfo](std::pair<ItemInfo, bool> c) {

						return (c.first.Location == tmpInfo.Location && c.second)


							; }), unwantedVecItems.end());


					pInterface->Inventory_AddItem(i, tmpInfo);
					houses[houseIndex].second++;
					pBlackboard->ChangeData("housesKnownLocations", houses);
					pBlackboard->ChangeData("InventoryChanged", true);
					pBlackboard->ChangeData("Inventory", inventory);
					pBlackboard->ChangeData("VectorUnwantedItems", unwantedVecItems);
					return Success;
				}
			}

		}
	}





	return Success;
}



//GoToUnsearchedHouse
BehaviorState GoToUnsearchedHouse(Elite::Blackboard* pBlackboard)
{
	ISteeringBehavior** pBehavior;
	Elite::Vector2** pTarget{};
	IExamInterface* pInterface = nullptr;
	bool ssSearchingHouse{ false };
	std::vector<std::pair<Vector2, int>> houses;
	pBlackboard->GetData("housesKnownLocations", houses);
	pBlackboard->GetData("IsSearchingHouse", ssSearchingHouse);
	pBlackboard->ChangeData("isInExplorerMode", false);


	auto dataAvailable = pBlackboard->GetData("SteeringBehavior", pBehavior) && pBlackboard->GetData("pTarget", pTarget) && pBlackboard->GetData("pInterface", pInterface);

	if (!dataAvailable)
	{
		return Failure;
	}


	AgentInfo agent = pInterface->Agent_GetInfo();

	auto tmp = std::min_element(houses.begin(), houses.end(), [agent](std::pair<Vector2, int> a, std::pair<Vector2, int> b) {

		if (a.second == 0 && b.second == 0)
		{
			return agent.Position.Distance(a.first) < agent.Position.Distance(b.first);
		}

		return a.second == 0;

		//return a.second < b.second;

		; });
	if (tmp != houses.end() && tmp->second == 0)
	{
		**pTarget = pInterface->NavMesh_GetClosestPathPoint((tmp->first));
		delete* pBehavior;
		*pBehavior = new Seek();
		ssSearchingHouse = true;
		pBlackboard->ChangeData("IsSearchingHouse", ssSearchingHouse);
		std::cout << "GOTOHOUSE!!!!\n";/////////////
		return Success;
	}
	else if (tmp == houses.end() || tmp->second != 0)
	{
		pBlackboard->ChangeData("IsSearchingHouse", false);
		return Failure;
	}
	else
	{
		return Success;
	}




	for (std::pair<Vector2, int>& a : houses)
	{
		if (a.second == 0)//can be better
		{

			if (**pTarget != a.first)
			{
				**pTarget = pInterface->NavMesh_GetClosestPathPoint((a.first));
				delete* pBehavior;
				*pBehavior = new Seek();
				return Success;
			}
			break;
		}
	}


	return Failure;


}
BehaviorState ManageInventory(Elite::Blackboard* pBlackboard)
{
	std::unordered_map<std::string, int> inventory{};
	pBlackboard->GetData("Inventory", inventory);
	IExamInterface* pInterface = nullptr;
	pBlackboard->GetData("pInterface", pInterface);
	inventory.clear();
	ItemInfo tmpItem{};
	std::cout << "ManagingInventory!!\n";

	for (size_t i = 0; i < pInterface->Inventory_GetCapacity(); i++)
	{
		if (pInterface->Inventory_GetItem(i, tmpItem))
		{
			switch (tmpItem.Type)
			{
			case eItemType::PISTOL:
				if (pInterface->Weapon_GetAmmo(tmpItem) < 1)
				{
					//	inventory["Pistol"]--;
					pInterface->Inventory_RemoveItem(i);
				}
				else
				{
					inventory["Pistol"]++;
				}
				break;
			case eItemType::MEDKIT:
				if (pInterface->Medkit_GetHealth(tmpItem) < 1)
				{
					//inventory["Medkit"]--;
					pInterface->Inventory_RemoveItem(i);
				}
				else
				{
					inventory["Medkit"]++;
				}
				break;
			case eItemType::FOOD:
				if (pInterface->Food_GetEnergy(tmpItem) < 1)
				{
					//inventory["Food"]--;
					pInterface->Inventory_RemoveItem(i);
				}
				else
				{
					inventory["Food"]++;
				}
				break;
			case eItemType::GARBAGE:
				inventory["Garbage"]--;
				pInterface->Inventory_RemoveItem(i);
				break;
			}
		}
	}
	pBlackboard->ChangeData("InventoryChanged", false);
	pBlackboard->ChangeData("Inventory", inventory);
	return Success;
}

BehaviorState Heal(Elite::Blackboard* pBlackboard)
{
	std::unordered_map<std::string, int> inventory{};
	pBlackboard->GetData("Inventory", inventory);
	IExamInterface* pInterface = nullptr;
	pBlackboard->GetData("pInterface", pInterface);
	ItemInfo tmpItem{};

	std::cout << "HEaling!!\n";



	for (size_t i = 0; i < pInterface->Inventory_GetCapacity(); i++)
	{
		if (pInterface->Inventory_GetItem(i, tmpItem))
		{
			if (tmpItem.Type == eItemType::MEDKIT)
			{
				pInterface->Inventory_UseItem(i);
				//inventory["Medkit"]--;
				pBlackboard->ChangeData("Inventory", inventory);
				pBlackboard->ChangeData("InventoryChanged", true);
				return Success;
			}
		}
	}


	std::cout << "HEaling FAILED!!\n";
	return Failure;
}




BehaviorState Eat(Elite::Blackboard* pBlackboard)
{
	std::unordered_map<std::string, int> inventory{};
	pBlackboard->GetData("Inventory", inventory);
	IExamInterface* pInterface = nullptr;
	pBlackboard->GetData("pInterface", pInterface);
	ItemInfo tmpItem{};


	std::cout << "EAting!!\n";

	for (size_t i = 0; i < pInterface->Inventory_GetCapacity(); i++)
	{
		if (pInterface->Inventory_GetItem(i, tmpItem))
		{
			if (tmpItem.Type == eItemType::FOOD)
			{
				pInterface->Inventory_UseItem(i);
				//inventory["Food"]--;
				pBlackboard->ChangeData("Inventory", inventory);
				pBlackboard->ChangeData("InventoryChanged", true);
				return Success;
			}
		}
	}
	std::cout << "EAting FAILED!!\n";
	return Failure;
}
BehaviorState AimAtZombie(Elite::Blackboard* pBlackboard)
{
	bool* pIsAiming = nullptr;
	IExamInterface* pInterface = nullptr;
	std::vector<Vector2>* pVecFOVEnemiesHash = nullptr;
	Elite::Vector2 target{};
	ISteeringBehavior** pBehavior;
	float* pAngVel = nullptr;
	pBlackboard->GetData("SteeringBehavior", pBehavior);
	pBlackboard->GetData("pInterface", pInterface);
	pBlackboard->GetData("VectorFOVEnemiesPos", pVecFOVEnemiesHash);
	pBlackboard->GetData("IsAiming", pIsAiming);
	pBlackboard->GetData("differenceAngluarVel", pAngVel);//
	AgentInfo agent = pInterface->Agent_GetInfo();




	if (pVecFOVEnemiesHash->size() < 1)
	{
		std::cout << "Zombies not seen\n";
		return Failure;
	}






	//might be best to pick multple people
	(target) = *std::min_element(pVecFOVEnemiesHash->begin(), pVecFOVEnemiesHash->end(), [&agent](const Vector2& a, const Vector2& b) {

		return (agent.Position.Distance(a)) <= (agent.Position.Distance(b))
			; });




	float degrees{ atan2(target.y, target.x) };
	auto offset = ToRadians(90.05001f);
	//float degrees{ atan2(target.x, target.y) };

	if (agent.Orientation < degrees + offset && agent.Orientation > degrees - offset || agent.Position.Distance(target) < agent.GrabRange)
	{
		std::cout << "SHOOOTING!!\n";
		*pIsAiming = false;
		return Shoot(pBlackboard);
		//shoot!
	}
	else if (agent.Orientation < degrees /*+ offset*/)
	{


		*pAngVel = -agent.MaxAngularSpeed;
	}
	else if (agent.Orientation > degrees /*+ offset*/)
	{
		*pAngVel = agent.MaxAngularSpeed;
	}
	std::cout << "AimingAtZOmbie!!\n";
	*pIsAiming = true;

	return Success;
}


BehaviorState Shoot(Elite::Blackboard* pBlackboard)
{

	std::unordered_map<std::string, int> inventory{};
	pBlackboard->GetData("Inventory", inventory);
	IExamInterface* pInterface = nullptr;
	pBlackboard->GetData("pInterface", pInterface);

	ItemInfo tmpItem{};
	for (size_t i = 0; i < pInterface->Inventory_GetCapacity(); i++)
	{
		if (pInterface->Inventory_GetItem(i, tmpItem))
		{
			if (tmpItem.Type == eItemType::PISTOL)
			{
				pInterface->Inventory_UseItem(i);
				//inventory["Pistol"]--;
				pBlackboard->ChangeData("Inventory", inventory);//maybe best to check and delete?
				pBlackboard->ChangeData("InventoryChanged", true);
				return Success;
			}
		}
	}
	return Failure;
}


BehaviorState Run(Elite::Blackboard* pBlackboard)
{
	bool** canRun = nullptr;
	IExamInterface* pInterface = nullptr;
	pBlackboard->GetData("canRun", canRun);
	pBlackboard->GetData("pInterface", pInterface);



	pBlackboard->ChangeData("prevFrameHp", pInterface->Agent_GetInfo().Health);

	if (pInterface->Agent_GetInfo().Stamina < 1)
	{
		*canRun = new bool(false);
		return Failure;
	}

	std::cout << "running!!\n";
	*canRun = new bool(true);
	return Success;
}
bool IsInventoryFull(Elite::Blackboard* pBlackboard)
{
	IExamInterface* pInterface = nullptr;
	pBlackboard->GetData("pInterface", pInterface);
	std::unordered_map<std::string, int> inventory{};
	pBlackboard->GetData("Inventory", inventory);
	int counter{ 0 };


	for (std::pair<std::string, int> a : inventory)
	{
		for (int i = 0; i < a.second; i++)
		{
			counter++;

		}

	}

	return  counter == pInterface->Inventory_GetCapacity();

}
bool IsInPurgeZone(Elite::Blackboard* pBlackboard)
{
	std::vector<PurgeZoneInfo>* VecPurgZone{};
	IExamInterface* pInterface = nullptr;
	pBlackboard->GetData("purgeZones", VecPurgZone);//
	pBlackboard->GetData("pInterface", pInterface);
	AgentInfo agent = pInterface->Agent_GetInfo();

	for (PurgeZoneInfo a : *VecPurgZone)
	{
		if (sqrt((a.Center.x - agent.Position.x) * (a.Center.x - agent.Position.x) + (a.Center.y - agent.Position.y) * (a.Center.y - agent.Position.y)) <= a.Radius)
		{
			pBlackboard->ChangeData("purgeZoneItIsIn", a);//
			return true;
		}

	}

	return false;
}

bool TooManyEnemiesInSight(Elite::Blackboard* pBlackboard)
{
	const size_t tooMuchEnemies{ 2 };
	std::vector<Vector2>* pVecFOVEnemiesHash = nullptr;
	pBlackboard->GetData("VectorFOVEnemiesPos", pVecFOVEnemiesHash);


	return pVecFOVEnemiesHash->size() >= tooMuchEnemies;
}

bool IsStuck(Elite::Blackboard* pBlackboard)
{
	bool* isStuck = nullptr;
	float* stuckTimer = nullptr;
	const float checkRadius{ 2.5f };
	const float maxTimer{ 2.f };
	IExamInterface* pInterface = nullptr;
	Vector2 stuckPoint{};
	pBlackboard->GetData("IsStuck", isStuck);
	pBlackboard->GetData("StuckCounter", stuckTimer);
	pBlackboard->GetData("pInterface", pInterface);
	pBlackboard->GetData("stuckPos", stuckPoint);//
	AgentInfo agent = pInterface->Agent_GetInfo();




	if (agent.Position.Distance(stuckPoint) < checkRadius)
	{
		*isStuck = new bool(true);
	}

	if (*stuckTimer > maxTimer)
	{
		*stuckTimer = 0;
		return true;
	}
	else if (agent.Position.Distance(stuckPoint) > checkRadius)
	{
		stuckPoint = agent.Position;
		pBlackboard->ChangeData("stuckPos", stuckPoint);//
		*stuckTimer = 0;
		*isStuck = new bool(false);
		return false;
	}
	else
	{

		return false;
	}


}





//extras
void AdjustLength(float& a)
{
	const float length{ 130.f };
	if (a > 0)
	{
		a = length;
	}
	else
	{
		a = -length;
	}
}











#endif