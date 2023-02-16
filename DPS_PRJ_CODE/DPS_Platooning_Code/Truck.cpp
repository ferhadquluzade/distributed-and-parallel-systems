#include "Truck.h"

Truck::Truck(int id, const SOCKADDR_IN& addr, int sequence):
	m_truckID(id),
	m_communicationAddr(addr),
	m_followSequence(sequence)
{

}

Truck::~Truck()
{
}

void Truck::setTruckVelocity(float v)
{
	m_currentV = v;
}

void Truck::setDistanceWithFrontTruck(float d)
{
	m_currentD = d;
}

void Truck::setCommunicationAddr(const SOCKADDR_IN& addr)
{
	m_communicationAddr = addr;
}

void Truck::setFollowingSequence(int sequence)
{
	m_followSequence = sequence;
}

int Truck::TruckID()
{
	return m_truckID;
}

SOCKADDR_IN Truck::getCommunicationAddr()
{
	return m_communicationAddr;
}

float Truck::getCurrentVelocity()
{
	return m_currentV;
}

float Truck::getCurrentDistanceWithFrontTruck()
{
	return m_currentD;
}

int Truck::getFollowingSequence()
{
	return m_followSequence;
}
