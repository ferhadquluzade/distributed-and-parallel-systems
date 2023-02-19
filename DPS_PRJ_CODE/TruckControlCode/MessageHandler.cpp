#include "MessageHandler.h"

int MessageHandler::Handle(const SMessageFeedBack& msgFeedback, bool & wantToJoin)
{
	switch (msgFeedback.m_FeedbackType)
	{
	case 0:
		desiredVelovity = (float)msgFeedback.m_velocity / 100;
		if (desiredVelovity == 0)
			return 0;
		cout << "Velocity of Leading Truck:" << desiredVelovity << " m/h" << endl;
		cout << "Velocity of Following Truck:" << curVelocoty << " m/h" << endl;
		if (curVelocoty != 0)
			desiredDistance = MINIMUN_SPACING + desiredVelovity * (curDistance / curVelocoty);
		speedControl();
		cout << "Current Distance from the front truck: " << desiredDistance << "m" << endl << endl;
		break;
	case 1:
		if (msgFeedback.m_JoinAllowed == 1)
		{
			cout << "Recieved Feedback!!! Seccessful Join!!" << endl;
			followingSequence = msgFeedback.m_sequenceNo;
		}
		break;
	case 2:
		if (msgFeedback.m_LeaveAllowed == 1)
		{
			cout << "Recieved Feedback! Allowed to Leave!" << endl;
			cout << "Successfully leave!\nWhether Join agin(Yes/No):";
			string str;
			cin >> str;
			if (str == "Yes")
				wantToJoin = true;
			else
				return 1;
		}
		else
			cout << "Recieved Feedback! Not Allowed to Leave!Please keep folloing" << endl;
		break;
	default:
		break;
	}

	return 0;
}

void MessageHandler::speedControl()
{
	if (curVelocoty < desiredVelovity)
	{
		cout << "Speed Up!\n";
		m_vehicleCol->speedUp();
		if (desiredVelovity != 0)
			curVelocoty = desiredVelovity;
		curDistance += 0.2;
		
	}
	else if (curVelocoty > desiredVelovity)
	{
		cout << "Speed Down!\n";
		m_vehicleCol->speedDown();
		if (desiredVelovity != 0)
			curVelocoty = desiredVelovity;
		curDistance -= 0.2;
	}
}

void MessageHandler::setCurVelocity(float c_v)
{
	curVelocoty = c_v;
}

void MessageHandler::setCurDistance(float c_d)
{
	curDistance = c_d;
}

float MessageHandler::getCurVelocity()
{
	return curVelocoty;
}

float MessageHandler::getCurDistance()
{
	return curDistance;
}

int MessageHandler::getFollowingSequence()
{
	return followingSequence;
}
