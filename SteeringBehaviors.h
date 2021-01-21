/*=============================================================================*/
// Copyright 2017-2018 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringBehaviors.h: SteeringBehaviors interface and different implementations
/*=============================================================================*/
#ifndef ELITE_STEERINGBEHAVIORS
#define ELITE_STEERINGBEHAVIORS

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "SteeringHelpers.h"
struct AgentInfo;
using namespace Elite;

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringOutput CalculateSteering(float deltaT, AgentInfo* pAgent) = 0;



	//Seek Functions
	virtual void SetTarget(const TargetData& target) { m_Target = target; }

	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{ return static_cast<T*>(this); }

protected:
	TargetData m_Target;

};
#pragma endregion

///////////////////////////////////////
//SEEK
//****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringOutput CalculateSteering(float deltaT, AgentInfo* pAgent) override;


};

//////////////////////////
//WANDER
//******
class Wander : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	//Wander Behavior
	SteeringOutput CalculateSteering(float deltaT, AgentInfo* pAgent) override;

	void SetWanderOffset(float offset) { m_Offset = offset; };
	void SetWanderRadius(float radius) { m_Radius = radius; };
	void SetMaxAngleChange(float rad) { m_AngleChange = rad; };

protected:
	float m_Offset = 8.f;
	float m_Radius = 15.f;
	float m_AngleChange = ToRadians(45);
//	float m_AngleChange = 3.f;
	//float m_AngleChange = ToRadians(randomFloat(360));
	float m_WanderAngle = 0.f;

private:
	/*void SetTarget(const TargetData* target)override {};*/

};

//////////////////////////
//FLEE
//******
class Flee : public Seek
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	//Wander Behavior
	SteeringOutput CalculateSteering(float deltaT, AgentInfo* pAgent) override;

private:
	float m_FleeRadius = 15.f;
};

//////////////////////////
//ARRIVE
//******
class Arrive : public Seek
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;

	//Wander Behavior
	SteeringOutput CalculateSteering(float deltaT, AgentInfo* pAgent) override;
private:
	const float m_MaxDistance = 7.5f;
};

//////////////////////////
//FACE
//******
class Face : public Seek
{
public:
	Face() = default;
	virtual ~Face() = default;

	//Wander Behavior
	SteeringOutput CalculateSteering(float deltaT, AgentInfo* pAgent) override;

private:

	const float m_BigDivider = 100000.f;
};
//////////////////////////
//PURSUIT
//******
class Pursuit : public Seek
{
public:
	Pursuit() = default;
	virtual ~Pursuit() = default;

	//Wander Behavior
	SteeringOutput CalculateSteering(float deltaT, AgentInfo* pAgent) override;
private:
	const float m_MaxDistance = 15.f;
	const float m_StepsInFuture = 3.f;
};
//////////////////////////
//EVADE
//******
class Evade : public Flee
{
public:
	Evade() = default;
	virtual ~Evade() = default;

	//Wander Behavior
	SteeringOutput CalculateSteering(float deltaT, AgentInfo* pAgent) override;

};

class Dodge : public Seek
{
public:
	Dodge() = default;
	virtual ~Dodge() = default;

	//Wander Behavior
	SteeringOutput CalculateSteering(float deltaT, AgentInfo* pAgent) override;

};
#endif


