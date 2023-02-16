#pragma once
#include <Ws2tcpip.h>

using namespace std;

class Truck
{
public:
	Truck(int id, const SOCKADDR_IN& addr, int sequence);

	~Truck();

	void setTruckVelocity(float v);

	void setDistanceWithFrontTruck(float d);

	void setCommunicationAddr(const SOCKADDR_IN& addr);

	void setFollowingSequence(int sequence);

	int TruckID();

	SOCKADDR_IN getCommunicationAddr();

	float getCurrentVelocity();

	float getCurrentDistanceWithFrontTruck();

	int getFollowingSequence();

private:
	int m_truckID;
	SOCKADDR_IN m_communicationAddr;
	int m_followSequence;
	float m_currentV;
	float m_currentD;
};

