#pragma once
#include "Truck.h"
#include <Ws2tcpip.h>
#include <map>
#include <list>
#include <mutex>

using namespace std;

class PlatooningManager
{
public:
	static PlatooningManager* getInstance();

private:
	PlatooningManager();
	~PlatooningManager();

public:
	bool addTruck(int id, Truck* truck);

	Truck* getTruck(int id);

	bool removeTruck(int id);

	int getNumOfFollowingTruck();

	void updateSequenceNo(int startSequence);

	list<SOCKADDR_IN*> getCommunicationAddr();

private:
	map<int, Truck*> m_trucksMap;
	mutex _mtx;
};

