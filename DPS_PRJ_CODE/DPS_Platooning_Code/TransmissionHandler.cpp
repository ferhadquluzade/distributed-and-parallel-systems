#include "TransmissionHandler.h"
#include <thread>

#define PORT_UDP 11500

TransmissionHandler::TransmissionHandler()
{

}

TransmissionHandler::~TransmissionHandler()
{
	closesocket(sockSrv);
	WSACleanup();
}

void TransmissionHandler::NetworkConfiguration()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		cout << "Socket Lib Configuration Failed£¡" << endl;
	}

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return;
	}

	printf("Sever is operating!\n\n");
	sockSrv = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN addrSrv;     
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PORT_UDP);

	if (bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		cout << "udp binding failed!";
	}
	
	
}

void TransmissionHandler::ThreadInitialization()
{
	/*thread Configuration*/
	thread sendThread(&TransmissionHandler::sendMsg, this);
	sendThread.detach();
}

void TransmissionHandler::sendMsg()
{
	Sleep(3000);
	int len = sizeof(SOCKADDR), t = 0;
	SMessageFeedBack feedback;
	feedback.m_FeedbackType = FeedBackType::CONTROL_FEEDBACK;
	while (1)
	{
		 if (UDPAddressList.empty())
			 continue;
		 float leadingVelocity = 40 + 5 * sin(0.02 * t);
		 feedback.m_velocity = leadingVelocity * 100;
			 
		 for (auto iter = UDPAddressList.begin(); iter != UDPAddressList.end(); iter++)
		 {
			 memcpy(sendBuf, &feedback, sizeof(SMessageFeedBack));
			 SOCKADDR_IN addr = *iter;
			 sendto(sockSrv, sendBuf, 100, 0, (SOCKADDR*)&addr, len);
		 }
		 Sleep(2000);
		 t++;
	}
}

void TransmissionHandler::sendMsgOnce(const SMessageFeedBack& msg)
{
	int len = sizeof(SOCKADDR);

	memcpy(sendBuf, &msg, sizeof(SMessageFeedBack));

	sendto(sockSrv, sendBuf, 100, 0, (SOCKADDR*)&addrClient, len);
}

void TransmissionHandler::sendMsgThreadExecution()
{
	int len = sizeof(SOCKADDR);
	while (1) {
		unique_lock <std::mutex> lck(_mtx);
		while (!_wakeUp)
			_cv.wait(lck);

		memcpy(sendBuf, &truckInfo, sizeof(SFollowingTruckInfo));

		sendto(sockSrv, sendBuf, 100, 0, (SOCKADDR*)&addrSrv, len);

		_wakeUp = false;
	}
}

SOCKADDR_IN TransmissionHandler::clientAddr()
{
	return addrClient;
}

SFollowingTruckInfo TransmissionHandler::recvMsg()
{
	int len = sizeof(SOCKADDR);
	SFollowingTruckInfo msg;
	recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
	memcpy(&msg, recvBuf, sizeof(SFollowingTruckInfo));
	if (msg.m_request == RequestType::JOIN_REQUEST)
		UDPAddressList.push_back(addrClient);
	else if (msg.m_request == RequestType::LEAVE_REQUEST)
	{
		auto iter = find(UDPAddressList.begin(), UDPAddressList.end(), addrClient);
		if (iter != UDPAddressList.end())
			UDPAddressList.erase(iter);
	}

	return msg;
}
