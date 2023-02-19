#include "MessageHandler.h"

void MessageHandler::ThreadInitialization()
{
	thread msgHandleThread(&MessageHandler::msgHandleExecution, this);
	msgHandleThread.detach();
}

void MessageHandler::OnMessage(const SFollowingTruckInfo& truckInfo, const SOCKADDR_IN& addr)
{
	m_truckInfo = truckInfo;
	m_addrClient = addr;
	if (!m_wakeUp)
	{
		m_wakeUp = true;
		m_cv.notify_one();
	}
}

void MessageHandler::msgHandleExecution()
{
	int len = sizeof(SOCKADDR);
	while (1) {
		unique_lock <std::mutex> lck(m_mtx);
		while (!m_wakeUp)
			m_cv.wait(lck);

		SMessageFeedBack feedbackMsg;
		float velocity = (float)m_truckInfo.m_velocity / 100;
		switch (m_truckInfo.m_request)
		{
			case RequestType::CONTROL_REQUEST:
				feedbackMsg.m_FeedbackType = FeedBackType::CONTROL_FEEDBACK;
				cout << "\n";
				cout << "Received the control request from Truck <" << m_truckInfo.m_truckID << "> " << endl;
				cout << "		Current Velocity is : " << velocity <<"km/h"<<endl;

				break;
			case RequestType::JOIN_REQUEST:
				cout << "\n";
				cout << "Received the join request from Truck <" << m_truckInfo.m_truckID << ">" << endl;
				feedbackMsg.m_FeedbackType = FeedBackType::JOIN_FEEDBACK;
				if (JoinPermissionCheck())
				{
					cout << "Permit to Join the platoon!" << endl;
					feedbackMsg.m_JoinAllowed = Permission::ALLOWED;
					int squence = PlatooningManager::getInstance()->getNumOfFollowingTruck() + 1;
					Truck* newTruck = new Truck(m_truckInfo.m_truckID, m_addrClient, squence);
					PlatooningManager::getInstance()->addTruck(m_truckInfo.m_truckID, newTruck);
					cout << "Successfully join to the platoon!" << endl;
				}
				else
					feedbackMsg.m_JoinAllowed = Permission::REFUSED;
				break;
			case RequestType::CONTROL_REQUEST:
				feedbackMsg.m_FeedbackType = FeedBackType::CONTROL_FEEDBACK;
				cout << "\n";
				cout << "Received the control request from Truck <" << m_truckInfo.m_truckID << "> " << endl;
				cout << "		Current Velocity is : " << velocity <<"km/h"<<endl;

				break;	
			case RequestType::LEAVE_REQUEST:
				cout << "\n";
				cout << "Received the leave request from Truck <" << m_truckInfo.m_truckID << ">" << endl;
				feedbackMsg.m_FeedbackType = FeedBackType::LEAVE_FEEDBACK;
				if (LeavePermissionCheck())
				{
					cout << "Permit to leave the platoon!" << endl;
					feedbackMsg.m_LeaveAllowed = Permission::ALLOWED;
					int curSequence = m_truckInfo.m_followingSequenceNo;
					PlatooningManager::getInstance()->updateSequenceNo(curSequence);
				}
				else
					m_feedbak.m_LeaveAllowed = Permission::REFUSED;
				break;
			case RequestType::CONTROL_REQUEST:
				feedbackMsg.m_FeedbackType = FeedBackType::CONTROL_FEEDBACK;
				cout << "\n";
				cout << "Received the control request from Truck <" << m_truckInfo.m_truckID << "> " << endl;
				cout << "		Current Velocity is : " << velocity <<"km/h"<<endl;

				break;
			default:
				break;
		}
		m_feedbak = feedbackMsg;

		m_wakeUp = false;
	}
}

bool MessageHandler::JoinPermissionCheck()
{
	return true;
}

bool MessageHandler::LeavePermissionCheck()
{
	return true;
}

SMessageFeedBack MessageHandler::getHandlingFeedBack()
{
	return m_feedbak;
}

