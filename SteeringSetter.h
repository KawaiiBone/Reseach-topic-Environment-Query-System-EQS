#pragma once
#include "SteeringBehaviors.h"
struct SteeringSetter
{
	ISteeringBehavior* steering{new Seek()};

	//SteeringSetter( ISteeringBehavior s):

	void SetToWander() {

		//steering = new Seek();
		//delete steering;
		steering = new Wander();
	};

	void SetToSeek() {
		//delete steering;
		steering = new Seek();
	};

	void SetToEvade() {
		//delete steering;
		steering = new Evade();
	};

	void SetToFlee() {
		//delete steering;
		steering = new Evade();
	};


	~SteeringSetter()
	{
		SAFE_DELETE(steering);
	}
};