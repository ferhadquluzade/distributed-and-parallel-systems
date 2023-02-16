#include "PlatooningManager.h"

PlatooningManager* PlatooningManager::getInstance()
{
    static PlatooningManager ins;
    return &ins;
}

PlatooningManager::PlatooningManager()
{
}

PlatooningManager::~PlatooningManager()
{
}

bool PlatooningManager::addTruck(int id, Truck* truck)
{
    if (truck)
    {
        _mtx.lock();
        m_trucksMap.insert(pair<int, Truck*>(id, truck));
        _mtx.unlock();
        return true;
    }
    return false;
}

Truck* PlatooningManager::getTruck(int id)
{
    lock_guard<mutex> lg(_mtx);
    auto iter = m_trucksMap.find(id);
    if (iter != m_trucksMap.end())
        return iter->second;

    return nullptr;
}

bool PlatooningManager::removeTruck(int id)
{
    lock_guard<mutex> lg(_mtx);
    auto iter = m_trucksMap.find(id);
    if (iter != m_trucksMap.end())
    {
        m_trucksMap.erase(iter);
        return true;
    }
    return false;
}

int PlatooningManager::getNumOfFollowingTruck()
{
    _mtx.lock();
    int num = m_trucksMap.size();
    _mtx.unlock();
    return num;
}

void PlatooningManager::updateSequenceNo(int startSequence)
{
    _mtx.lock();
    for (auto iter = m_trucksMap.begin(); iter != m_trucksMap.end(); iter++)
    {
        int curSequence = iter->second->getFollowingSequence();
        if (curSequence > startSequence)
        {
            iter->second->setFollowingSequence(curSequence - 1);
        }
    }
    _mtx.unlock();
}

list<SOCKADDR_IN*> PlatooningManager::getCommunicationAddr()
{
    list<SOCKADDR_IN*> addrs;
    _mtx.lock();
    for (auto iter = m_trucksMap.begin(); iter != m_trucksMap.end(); iter++)
    {
        SOCKADDR_IN addr = iter->second->getCommunicationAddr();
        addrs.push_back(&addr);
    }
    _mtx.unlock();
    return addrs;
}
