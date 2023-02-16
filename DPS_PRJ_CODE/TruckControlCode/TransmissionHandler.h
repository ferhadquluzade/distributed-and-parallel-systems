#pragma once
#include <Ws2tcpip.h>
#include <iostream>
#include "SFollowingTruckInfo.h"
#include "SMessageFeedBack.h"
#include <mutex>              
#include <condition_variable> 
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class TransmissionHandler
{
public:
	TransmissionHandler();
	~TransmissionHandler();

	void Configuration();

	void sendMsg(const SFollowingTruckInfo& msg);

	void sendMsgThreadExecution();

	SMessageFeedBack recvMsg();

public:
	SOCKADDR_IN addrClient;
	bool wantToLeave = false;
	bool wantToJoin = false;

	int len = sizeof(SOCKADDR);
	SOCKET sockSrv;
	sockaddr_in  addrSrv;
	SFollowingTruckInfo truckInfo;
	char recvBuf[100];
	char sendBuf[100];

	mutex _mtx;
	condition_variable _cv;
	bool _wakeUp = false;
};

