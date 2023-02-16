#include "TransmissionHandler.h"
#include <thread>

TransmissionHandler::TransmissionHandler()
{
}

TransmissionHandler::~TransmissionHandler()
{
	closesocket(sockSrv);
	WSACleanup();
}

void TransmissionHandler::Configuration()
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
	}

	printf("Client is operating!\n\n");
	sockSrv = socket(AF_INET, SOCK_DGRAM, 0);


	inet_pton(AF_INET, "192.168.178.48", &addrSrv.sin_addr.S_un.S_addr);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(11500);

	
	/*thread Configuration*/
	thread sendThread(&TransmissionHandler::sendMsgThreadExecution, this);
	sendThread.detach();
}

void TransmissionHandler::sendMsg(const SFollowingTruckInfo& msg)
{
	truckInfo = msg;
	//wake up thread
	if (!_wakeUp)
	{
		_wakeUp = true;
		_cv.notify_one();
	}
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

SMessageFeedBack TransmissionHandler::recvMsg()
{
	int len = sizeof(SOCKADDR);
	SMessageFeedBack msgFeedback;
	recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len);
	memcpy(&msgFeedback, recvBuf, sizeof(SMessageFeedBack));

	return msgFeedback;
}


//
