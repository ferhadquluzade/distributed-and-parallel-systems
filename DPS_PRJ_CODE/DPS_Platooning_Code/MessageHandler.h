#pragma once
#include "SMessageFeedBack.h"
#include "SFollowingTruckInfo.h"
#include "EGeneralEnum.h"
#include "Truck.h"
#include "PlatooningManager.h"
#include <iostream>
#include <mutex>
#include <Ws2tcpip.h>
#include <thread>

#define MINIMUN_SPACING 3

using namespace std;

class MessageHandler
{
public:
	void ThreadInitialization();

	void OnMessage(const SFollowingTruckInfo& truckInfo, const SOCKADDR_IN &addr);

	void msgHandleExecution();
	
	bool JoinPermissionCheck();

	bool LeavePermissionCheck();

	SMessageFeedBack getHandlingFeedBack();

private:
	SFollowingTruckInfo m_truckInfo;
	SOCKADDR_IN m_addrClient;
	SMessageFeedBack m_feedbak;

	mutex m_mtx;
	condition_variable m_cv;
	bool m_wakeUp = false;
};

