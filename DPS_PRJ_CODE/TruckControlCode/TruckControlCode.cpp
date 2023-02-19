// TruckControlCode.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Winsock2.h>
#include <stdio.h>
#include <Ws2tcpip.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")
#include "SFollowingTruckInfo.h"
#include "TransmissionHandler.h"
#include "MessageHandler.h"

using namespace std;

SMessageFeedBack msgFeedback;

bool wantToLeave = false;
bool wantToJoin = true;

SFollowingTruckInfo truckInfo;

int main()
{
	TransmissionHandler tansHandler;
	tansHandler.Configuration();
	MessageHandler msgHandler;
	clock_t curTime, tempTime, sendTime;
	int timeOut;

	cout << "Set Truck Details:\n"
		<< "Truck ID(int):";
	cin >> truckInfo.m_truckID;
	cout << "Current Velocity(km/h):";
	cin >> truckInfo.m_velocity;
	cout << "Current Distance(m) from leading truck:";
	cin >> truckInfo.m_distance;
	cout << "Following Time(s):";
	cin >> timeOut;
	msgHandler.setCurDistance(truckInfo.m_distance);
	msgHandler.setCurVelocity(truckInfo.m_velocity);

	curTime = clock();
	tempTime = clock();
	sendTime = clock();

	while (1)
	{
		curTime = clock();
		if ((curTime - tempTime) * 1000 / CLOCKS_PER_SEC > timeOut * 1000)
			wantToLeave = true;

		if (wantToJoin)
		{
			wantToJoin = false;
			truckInfo.m_request = 1;
			truckInfo.m_velocity = msgHandler.getCurVelocity();
			truckInfo.m_request = 0;
			truckInfo.m_velocity = 100 * msgHandler.getCurVelocity();
			tansHandler.sendMsg(truckInfo);
		}
		
		msgFeedback = tansHandler.recvMsg();
		int end = msgHandler.Handle(msgFeedback, wantToJoin);
		if (end == 1)
			break;

		if ((curTime - sendTime) * 1000 / CLOCKS_PER_SEC > 5000)//send per 2s
		{
			sendTime = clock();
			truckInfo.m_request = 0;
			truckInfo.m_velocity = 100 * msgHandler.getCurVelocity();
			truckInfo.m_distance = 100 * msgHandler.getCurDistance();
			tansHandler.sendMsg(truckInfo);
		}


		if (wantToLeave)
		{
			wantToLeave = false;
			tempTime = clock();
			truckInfo.m_request = 2;
			truckInfo.m_followingSequenceNo = msgHandler.getFollowingSequence();
			cout << "Send leave request to leading truck...\n";
			tansHandler.sendMsg(truckInfo);
		}
		
	}

}

