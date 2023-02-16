#pragma once
#include <Ws2tcpip.h>
#include <iostream>
#include "SFollowingTruckInfo.h"
#include "SMessageFeedBack.h"
#include "PlatooningManager.h"
#include "EGeneralEnum.h"
#include <mutex> 
#include <list>
#include <condition_variable> 
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class TransmissionHandler
{
public:
	TransmissionHandler();
	~TransmissionHandler();

	void NetworkConfiguration();

	void ThreadInitialization();

	void sendMsg();

	void sendMsgOnce(const SMessageFeedBack& msg);

	void sendMsgThreadExecution();

	SOCKADDR_IN clientAddr();

	SFollowingTruckInfo recvMsg();

public:
	SOCKADDR_IN addrClient;
	bool wantToLeave = false;
	bool wantToJoin = false;

	int len = sizeof(SOCKADDR);
	SOCKET sockSrv;
	sockaddr_in  addrSrv;
	list<SOCKADDR_IN> UDPAddressList;
	SFollowingTruckInfo truckInfo;
	char recvBuf[100];
	char sendBuf[100];

	mutex _mtx;
	condition_variable _cv;
	bool _wakeUp = false;
};

