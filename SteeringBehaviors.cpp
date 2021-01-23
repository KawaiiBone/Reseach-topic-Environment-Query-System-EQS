//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "Exam_HelperStructs.h"

//SEEK
//****
SteeringOutput Seek::CalculateSteering(float deltaT, AgentInfo* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = (m_Target).Position - pAgent->Position; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= pAgent->MaxLinearSpeed; //Rescale to Max Speed

	//DEBUG RENDERING
	//if (pAgent->CanRenderBehavior())
	//	DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 }, 0.4f);

	return steering;
}

//WANDER (base> SEEK)
//******
SteeringOutput Wander::CalculateSteering(float deltaT, AgentInfo* pAgent)
{

	//std::cout << RandomAngle << "\n";
	//m_Target.Clear();
	//std::cout << "Wanderangle: " << m_AngleChange << "\n";
	SteeringOutput steering = {};
	Elite::Vector2 LinearVelocity{ pAgent->LinearVelocity.GetNormalized() };

	Elite::Vector2 circlePos{ LinearVelocity * (m_Offset) + pAgent->Position };

	m_WanderAngle = randomFloat(m_AngleChange*2)-m_AngleChange;

	Elite::Vector2 randomOffset{ cos(m_WanderAngle) * (m_Radius) ,sin(m_WanderAngle) * m_Radius };
	//std::cout << "randomOffset: " << randomOffset.x << "   " << randomOffset.y << "\n";
	randomOffset += circlePos;


	//std::cout << "Wanderangle: " << m_WanderAngle << "\n";





	m_Target.Position = randomOffset;

	steering = { Seek::CalculateSteering(deltaT, pAgent) };



	//DEBUG RENDERING
	//if (pAgent->CanRenderBehavior())
	//{
	///DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 }, 0.4f);
	//	DEBUGRENDERER2D->DrawCircle(Elite::Vector2{ circlePos  }, m_Radius, Elite::Color(0, 1, 1, 1), 0);
	//}


	return steering;
}

//Flee (base> SEEK)
//******
SteeringOutput Flee::CalculateSteering(float deltaT, AgentInfo* pAgent)
{
	auto distanceToTarget = Distance(pAgent->Position, m_Target.Position);

	//if (distanceToTarget > m_FleeRadius)
	//{
	//	SteeringOutput steering{};
	//	steering.IsValid = false;
	//	return steering;
	//}


	SteeringOutput steering = { Seek::CalculateSteering(deltaT, pAgent) };
	steering.LinearVelocity *= -1;
	return steering;
}

//Arrive (base> SEEK)
//******
SteeringOutput Arrive::CalculateSteering(float deltaT, AgentInfo* pAgent)
{

	SteeringOutput steering = { Seek::CalculateSteering(deltaT, pAgent) };

	float distance{pAgent->Position.Distance(m_Target.Position)};

	if (distance <  m_MaxDistance)
	{
		steering.LinearVelocity *= distance * deltaT;
	}


	return steering;
}

//Face (base> SEEK)
//******
//SteeringOutput Face::CalculateSteering(float deltaT, AgentInfo* pAgent)
//{
//	pAgent->SetAutoOrient(false);
//	SteeringOutput steering = {  };
//
//	float degrees{atan2(m_Target.Position.x, -m_Target.Position.y)};
//
//	if (pAgent->GetRotation() > degrees)
//	{
//		steering.AngularVelocity = -1;
//	}
//	else if (pAgent->GetRotation() < degrees)
//	{
//		steering.AngularVelocity = +1;
//	}
//	else 
//	{
//		steering.AngularVelocity = 0;
//	}
//	return steering;
//}







//Pursuit (base> SEEK)
//******
SteeringOutput Pursuit::CalculateSteering(float deltaT, AgentInfo* pAgent)
{
	//Elite::Vector2 position{ m_Target.Position + m_Target.LinearVelocity * 3 };



	float distance{ pAgent->Position.Distance(m_Target.Position + m_Target.LinearVelocity * m_StepsInFuture) };


	if (distance > m_MaxDistance)
	{
		m_Target.Position = m_Target.Position + m_Target.LinearVelocity * m_StepsInFuture;
	}
	else
	{
		//std::cout << "aaa\n";
	}

	SteeringOutput steering = { Seek::CalculateSteering(deltaT, pAgent) };

	return steering;
}

//Evade (base> SEEK)
//******
SteeringOutput Evade::CalculateSteering(float deltaT, AgentInfo* pAgent)
{
	SteeringOutput steering = { Flee::CalculateSteering(deltaT, pAgent) };
	//steering.LinearVelocity *= -1;
	return steering;
}
