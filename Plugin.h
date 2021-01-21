#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"

//self stuff
#include "Behaviors.h"
#include "SteeringBehaviors.h"
#include <unordered_map>


class IBaseInterface;
class IExamInterface;

class Plugin :public IExamPlugin
{
public:



	Plugin() {};
	virtual ~Plugin();


	void Initialize(IBaseInterface* pInterface, PluginInfo& info) override;
	void DllInit() override;
	void DllShutdown() override;

	void InitGameDebugParams(GameDebugParams& params) override;
	void Update(float dt) override;

	SteeringPlugin_Output UpdateSteering(float dt) override;
	void Render(float dt) const override;

private:
	//Interface, used to request data from/perform actions with the AI Framework
	IExamInterface* m_pInterface = nullptr;
	vector<HouseInfo> GetHousesInFOV() const;
	vector<EntityInfo> GetEntitiesInFOV() const;

	Elite::Vector2 m_Target = {};
	bool m_CanRun = false; //Demo purpose
	bool m_GrabItem = false; //Demo purpose
	bool m_UseItem = false; //Demo purpose
	bool m_RemoveItem = false; //Demo purpose
	float m_AngSpeed = 0.f; //Demo purpose

	//self stuff
	//var
	AgentInfo* m_pAgentInfo = nullptr;
	Elite::IDecisionMaking* m_DecisionMaking = nullptr;
	ISteeringBehavior* m_pSteeringBehavior = nullptr;
	std::vector<Vector2> m_EnemiesFOVLocation;
	std::vector<EntityInfo> m_ItemsFOV;
	Vector2* m_pTarget = new Vector2(1,1);
	bool* m_pCanRun{ new bool(false) };
	bool m_IsClockWise{false};
	Vector2* m_pExploreLocation = new Vector2(1, 1);
	bool m_IsAiming{ false };
	float m_DifferenceAngluarVel{0.f};
	float m_ScaredTimer{0.f};
	bool m_IsScared{false};
	std::vector<PurgeZoneInfo> m_vecPurgeZones;


	//the int is to see how much value the location has
	// 0 is unchecked, -1 is empty(so it will always stay empty), 1 or above means how much items has found there(for each item +1)
	std::vector<std::pair<Vector2, int>> m_HousesKnownLocations{};



	//functions
	Blackboard* CreateBlackboard();
	void FirstExploreLocation();
	void AddHousesInFOV();



	






};
//ENTRY
//This is the first function that is called by the host program
//The plugin returned by this function is also the plugin used by the host program
extern "C"
{
	__declspec (dllexport) IPluginBase* Register()
	{
		return new Plugin();
	}
}


