#pragma once
#include "SMessageFeedBack.h"
#include "VehicleControl.h"
#include <iostream>

#define MINIMUN_SPACING 3

using namespace std;

class MessageHandler
{
public:
	int Handle(const SMessageFeedBack & msgFeedback, bool& wantToJoin);
	void speedControl();

	void setCurVelocity(float c_v);
	void setCurDistance(float c_d);
	float getCurVelocity();
	float getCurDistance();
	int getFollowingSequence();

private:
	VehicleControl* m_vehicleCol;
	float	desiredVelovity = 0;
	float	curVelocoty = 0;
	float	curDistance = 0;
	float	desiredDistance = 0;
	int		followingSequence;
};

