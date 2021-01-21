#include "stdafx.h"
#include "Plugin.h"
#include "IExamInterface.h"

Plugin::~Plugin()
{
	SAFE_DELETE(m_DecisionMaking);
	//SAFE_DELETE(m_pAgentInfo);
	//SAFE_DELETE(m_pTarget);
	SAFE_DELETE(m_pExploreLocation);
	SAFE_DELETE(m_pCanRun);
}

//Called only once, during initialization
void Plugin::Initialize(IBaseInterface* pInterface, PluginInfo& info)
{
	//Retrieving the interface
	//This interface gives you access to certain actions the AI_Framework can perform for you
	m_pInterface = static_cast<IExamInterface*>(pInterface);
	//std::cout << m_pInterface->World_GetInfo().Dimensions.x << "\n";
	//Bit information about the plugin
	//Please fill this in!!
	FirstExploreLocation();
	info.BotName = "BotNameTEST";
	info.Student_FirstName = "Mathias";
	info.Student_LastName = "Radder";
	info.Student_Class = "2DA07N";



	//m_SteeringSetter =  SteeringSetter();
	m_pSteeringBehavior = new Seek();

	Blackboard* pB = CreateBlackboard();


	BehaviorTree* pBqehavior = new BehaviorTree(pB,
		{ new BehaviorSelector(
			{

				new BehaviorSequence({
						new BehaviorConditional(HasInventoryChanged),
						new BehaviorAction(ManageInventory),
					}),
					new BehaviorSequence({
						new BehaviorConditional(IsInPurgeZone),
						new BehaviorAction(RunOutOfPurgeZone),
					}),
					/*new BehaviorSequence(
					{
						new BehaviorConditional(IsInventoryFull),
						 new BehaviorSelector({
							 new BehaviorSequence({
									new BehaviorConditional(AreItemsInGrabRange),
									new BehaviorAction(GrabAndAddItems),
								}),
							 new BehaviorSequence({
							new BehaviorConditional(AreItemsInSight),
							new BehaviorAction(SeekClosestItem),
							}),
						
							 }),
					}),*/
				new BehaviorSequence({
						new BehaviorConditional(AreItemsInGrabRange),
						new BehaviorAction(GrabAndAddItems),
					}),
				new BehaviorSequence({
						new BehaviorConditional(AreItemsInSight),
						new BehaviorAction(SeekClosestItem),
								}),
				new BehaviorSequence(
					{
						new BehaviorConditional(AreEnemiesSight),
						 new BehaviorSelector({
							 new BehaviorSequence({
									new BehaviorConditional(HaveGunAndAmmo),
									new BehaviorAction(AimAtZombie),
								}),
							 new BehaviorSequence({
							new BehaviorConditional(TooManyEnemiesInSight),
							new BehaviorAction(ChangeToFlee),
							}),
						new BehaviorAction(ChangeToEvade)
							 }),
					}),
		/*		new BehaviorSequence({
						new BehaviorConditional(HasMedKit),
						new BehaviorConditional(IsHealthLow),
						new BehaviorAction(Heal),
					}),*/
								new BehaviorSequence({
						new BehaviorConditional(IsHealthLow),
						 new BehaviorSelector({
							 new BehaviorSequence({
									new BehaviorConditional(HasMedKit),
									new BehaviorAction(Heal),
								}),
						new BehaviorAction(GoToMedkit)
						 }),
					}),
				new BehaviorSequence({
						new BehaviorConditional(IsEnergyLow),
						 new BehaviorSelector({
							 new BehaviorSequence({
									new BehaviorConditional(HasFood),
									new BehaviorAction(Eat),
								}),
						new BehaviorAction(GoToFood)
						 }),
					}),
			new BehaviorSequence({
						new BehaviorConditional(HaveGunAndAmmo),
						new BehaviorAction(GoToGun),
					}),
				new BehaviorSequence({
						new BehaviorConditional(TookDMG),
						new BehaviorAction(Run),
					}),
					new BehaviorSequence(
					{
						new BehaviorConditional(HasNotCheckedHouse),
						 new BehaviorSelector({
							new BehaviorSequence({
									new BehaviorConditional(AreItemsInGrabRange),
									new BehaviorAction(GrabAndAddItems),

								}),
									new BehaviorSequence({
									new BehaviorConditional(AreItemsInSight),
									new BehaviorAction(SeekClosestItem),

								}),
								new BehaviorSequence({
									new BehaviorConditional(IsRoomEmpty),
									new BehaviorAction(ChangeToExploreMode)
								}),
							 /*new BehaviorAction(SeekClosestItem),*/
						 new BehaviorAction(GoToUnsearchedHouse)
					  }),
				 }),
			/*			new BehaviorSequence(
							{
								new BehaviorConditional(AreItemsInSight),
								new BehaviorAction(SeekClosestItem)
							}),*/
							new BehaviorSequence(
							{
								new BehaviorConditional(IsInExploreArea),
								new BehaviorAction(ChangeExploreLocation)
							}),

						new BehaviorAction(ChangeToExploreMode)
					})


		}
	);


	m_DecisionMaking = pBqehavior;


}
//Called only once
void Plugin::DllInit()
{
	//Called when the plugin is loaded
}

//Called only once
void Plugin::DllShutdown()
{
	//Called wheb the plugin gets unloaded
}

//Called only once, during initialization
void Plugin::InitGameDebugParams(GameDebugParams& params)
{
	params.AutoFollowCam = true; //Automatically follow the AI? (Default = true)
	params.RenderUI = true; //Render the IMGUI Panel? (Default = true)
//	params.SpawnEnemies = false;
	params.SpawnEnemies = true;//Do you want to spawn enemies? (Default = true)
	params.EnemyCount = 20; //How many enemies? (Default = 20)
	//params.EnemyCount = 0; //How many enemies? (Default = 20)
	params.GodMode = false; //GodMode > You can't die, can be usefull to inspect certain behaviours (Default = false)
	//
	params.AutoGrabClosestItem = false; //A call to Item_Grab(...) returns the closest item that can be grabbed. (EntityInfo argument is ignored)
}

//Only Active in DEBUG Mode
//(=Use only for Debug Purposes)
void Plugin::Update(float dt)
{
	//Demo Event Code
	//In the end your AI should be able to walk around without external input
	if (m_pInterface->Input_IsMouseButtonUp(Elite::InputMouseButton::eLeft))
	{
		//Update target based on input
		Elite::MouseData mouseData = m_pInterface->Input_GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
		const Elite::Vector2 pos = Elite::Vector2(static_cast<float>(mouseData.X), static_cast<float>(mouseData.Y));
		m_Target = m_pInterface->Debug_ConvertScreenToWorld(pos);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Space))
	{
		m_CanRun = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Left))
	{
		m_AngSpeed -= Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Right))
	{
		m_AngSpeed += Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_G))
	{
		m_GrabItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_U))
	{
		m_UseItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_R))
	{
		m_RemoveItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyUp(Elite::eScancode_Space))
	{
		m_CanRun = false;
	}
}

//Update
//This function calculates the new SteeringOutput, called once per frame
SteeringPlugin_Output Plugin::UpdateSteering(float dt)
{
	if (m_IsScared)
	{
		m_ScaredTimer += dt;
	}
	else
	{
		m_ScaredTimer = 0;
	}
	m_pAgentInfo = &m_pInterface->Agent_GetInfo();
	auto steering = SteeringPlugin_Output();
	//std::cout << m_pAgentInfo->Position.x << "\n";
	//Use the Interface (IAssignmentInterface) to 'interface' with the AI_Framework

	auto nextTargetPos = m_Target; //To start you can use the mouse position as guidance

	//auto vHousesInFOV = GetHousesInFOV();//uses m_pInterface->Fov_GetHouseByIndex(...)
	auto vEntitiesInFOV = GetEntitiesInFOV(); //uses m_pInterface->Fov_GetEntityByIndex(...)
	//AddHousesInFOV();



	for (auto& e : vEntitiesInFOV)
	{
		if (e.Type == eEntityType::ENEMY)
		{
			m_EnemiesFOVLocation.push_back(e.Location);

		}
		else if (e.Type == eEntityType::ITEM)//dit is beter in het behaviors.h
		{
			m_ItemsFOV.push_back(e);

		}
		else if (e.Type == eEntityType::PURGEZONE)
		{
			PurgeZoneInfo zoneInfo;
			m_pInterface->PurgeZone_GetInfo(e, zoneInfo);
			std::cout << "Purge Zone in FOV:" << e.Location.x << ", " << e.Location.y << " ---EntityHash: " << e.EntityHash << "---Radius: " << zoneInfo.Radius << std::endl;
			m_vecPurgeZones.push_back(zoneInfo);
		}
	}


	//INVENTORY USAGE DEMO
	//********************

	if (m_GrabItem)
	{
		ItemInfo item;
		//Item_Grab > When DebugParams.AutoGrabClosestItem is TRUE, the Item_Grab function returns the closest item in range
		//Keep in mind that DebugParams are only used for debugging purposes, by default this flag is FALSE
		//Otherwise, use GetEntitiesInFOV() to retrieve a vector of all entities in the FOV (EntityInfo)
		//Item_Grab gives you the ItemInfo back, based on the passed EntityHash (retrieved by GetEntitiesInFOV)
		if (m_pInterface->Item_Grab({}, item))
		{
			//Once grabbed, you can add it to a specific inventory slot
			//Slot must be empty
			m_pInterface->Inventory_AddItem(0, item);
		}
	}

	if (m_UseItem)
	{
		//Use an item (make sure there is an item at the given inventory slot)
		m_pInterface->Inventory_UseItem(0);
	}

	if (m_RemoveItem)
	{
		//Remove an item from a inventory slot
		m_pInterface->Inventory_RemoveItem(0);
	}




	m_DecisionMaking->Update(dt);
	//SetSteering(m_pSteeringBehavior);
	//m_pSteeringBehavior = m_SteeringSetter.steering;
	//std::cout << "im in pain help" << "\n";



	m_pSteeringBehavior->SetTarget(nextTargetPos);
	//std::cout << "mpTarget " << m_pTarget->x << "\n";
	m_pSteeringBehavior->SetTarget(*m_pTarget);
	steering.LinearVelocity = m_pSteeringBehavior->CalculateSteering(dt, &m_pInterface->Agent_GetInfo()).LinearVelocity;



	//return steering;
	//steering.AngularVelocity = m_AngSpeed; //Rotate your character to inspect the world while walking
	//steering.AutoOrient = true; //Setting AutoOrientate to TRue overrides the AngularVelocity

	if (m_IsAiming)
	{
		steering.AutoOrient = false;		 //Setting AutoOrientate to TRue overrides the AngularVelocity
		steering.AngularVelocity = m_DifferenceAngluarVel;
	}
	else
	{
		steering.AutoOrient = true;		 //Setting AutoOrientate to TRue overrides the AngularVelocity
		//steering.AngularVelocity = m_pAgentInfo->MaxAngularSpeed;
	}


	//steering.RunMode = m_CanRun; //If RunMode is True > MaxLinSpd is increased for a limited time (till your stamina runs out)
	steering.RunMode = *m_pCanRun;
	//SteeringPlugin_Output is works the exact same way a SteeringBehaviour output

	//@End (Demo Purposes)
	m_GrabItem = false; //Reset State
	m_UseItem = false;
	m_RemoveItem = false;
	m_EnemiesFOVLocation.clear();
	m_ItemsFOV.clear();
	m_vecPurgeZones.clear();
	return steering;
}

//This function should only be used for rendering debug elements
void Plugin::Render(float dt) const
{
	//This Render function should only contain calls to Interface->Draw_... functions
	m_pInterface->Draw_SolidCircle(m_Target, .7f, { 0,0 }, { 1, 0, 0 });
}

vector<HouseInfo> Plugin::GetHousesInFOV() const
{
	vector<HouseInfo> vHousesInFOV = {};

	HouseInfo hi = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetHouseByIndex(i, hi))
		{

			vHousesInFOV.push_back(hi);
			continue;
		}

		break;
	}

	return vHousesInFOV;
}



//important part of the generator
//Adds locations, the importance will be calculated somewhere else due to it still doesnt know
//how important that house location (many items or is empty)
void Plugin::AddHousesInFOV()
{
	HouseInfo hi = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetHouseByIndex(i, hi))
		{

			for (std::pair<Vector2, int>& a : m_HousesKnownLocations)
			{
				if (hi.Center == a.first)
				{
					return;
				}
			}
			m_HousesKnownLocations.push_back({ hi.Center,0 });
			//m_HousesKnownLocationspush_back({hi.Center,0});



			continue;
		}

		break;
	}


}

vector<EntityInfo> Plugin::GetEntitiesInFOV() const
{
	vector<EntityInfo> vEntitiesInFOV = {};

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetEntityByIndex(i, ei))
		{
			vEntitiesInFOV.push_back(ei);
			continue;
		}

		break;
	}

	return vEntitiesInFOV;
}

Blackboard* Plugin::CreateBlackboard()
{
	int tmpIndex{ 0 };
	std::vector<std::pair<Vector2, int>> tmp{ };
	bool tmpIsSearchHouse{ false };
	std::unordered_map<std::string, int> inventory{ {"Pistol",0}, {"Medkit",0}, {"Food",0} };
	Elite::Blackboard* pBlackboard = new Elite::Blackboard();
	std::vector<std::pair<ItemInfo, bool>> unwantedVecItems{};
	PurgeZoneInfo purgeZone;
	//pBlackboard->AddData("Agent", m_pAgentInfo);
	pBlackboard->AddData("Target", &m_Target);
	pBlackboard->AddData("pTarget", &m_pTarget);
	pBlackboard->AddData("pExploreTarget", &m_pExploreLocation);
	pBlackboard->AddData("isClockWise", m_IsClockWise);
	pBlackboard->AddData("isInExplorerMode", false);
	pBlackboard->AddData("isScared", &m_IsScared);
	pBlackboard->AddData("scaredTimer", &m_ScaredTimer);
	pBlackboard->AddData("canRun", &m_pCanRun);
	//	pBlackboard->AddData("SteeringSetter", &m_SteeringSetter);
	pBlackboard->AddData("SteeringBehavior", &m_pSteeringBehavior);
	pBlackboard->AddData("VectorFOVEnemiesPos", &m_EnemiesFOVLocation);
	pBlackboard->AddData("VectorFOVItems", &m_ItemsFOV);
	pBlackboard->AddData("VectorUnwantedItems", unwantedVecItems);
	pBlackboard->AddData("pInterface", m_pInterface);
	//pBlackboard->AddData("housesKnownLocations", m_HousesKnownLocations);
	pBlackboard->AddData("housesKnownLocations", tmp);
	pBlackboard->AddData("HouseIndex", tmpIndex);
	pBlackboard->AddData("IsSearchingHouse", false);
	pBlackboard->AddData("InventoryChanged", false);
	pBlackboard->AddData("Inventory", inventory);
	pBlackboard->AddData("IsAiming", &m_IsAiming);//m_DifferenceAngluarVel
	pBlackboard->AddData("differenceAngluarVel", &m_DifferenceAngluarVel);//
	pBlackboard->AddData("prevFrameHp", m_pInterface->Agent_GetInfo().Health);//
	pBlackboard->AddData("purgeZones", m_vecPurgeZones);//
	pBlackboard->AddData("purgeZoneItIsIn", purgeZone);//
	return pBlackboard;
}

//void Plugin::SetSteering(ISteeringBehavior* pST)
//{
//	//delete pST;
//	//pST = m_SteeringSetter.steering;
//}
void Plugin::FirstExploreLocation()
{
	srand(unsigned int(time(nullptr)));
	float randomAnge{ float(randomInt(360)) };
	int randomIntt{ randomInt(2) };
	randomAnge = ToRadians(randomAnge);
	//int randomIntY{ randomInt(2) };
	Vector2 length{ m_pInterface->World_GetInfo().Dimensions / 2 };
	////	std::cout << randomIntX << "\n";
	//	//std::cout << randomIntY << "\n"

	if (randomIntt == 0)
	{
		m_IsClockWise = true;
	}

	delete m_pExploreLocation;
	//m_pExploreLocation = new Vector2((length.x-10) * randomIntX, (length.y - 10) * randomIntY);
	m_pExploreLocation = new Vector2(cos(randomAnge) * (length.x), sin(randomAnge) * length.x);

	//std::cout << m_pExploreLocation->x << "\n";
	//std::cout << m_pExploreLocation->y << "\n";
}


