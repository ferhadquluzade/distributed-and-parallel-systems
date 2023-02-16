#pragma once

struct SFollowingTruckInfo
{
	//Frame head
	//SFrameHead m_frameHead;

	//Data
	//Unique identificcation of Truck
	int m_truckID;

	//the sequence number of truck following the leading truck
	//0 means leading truck
	int m_followingSequenceNo;

	//0;control request; 1 join request;2 leave request
	short m_request;

	//current moving speed of truck
	int m_velocity;

	//distance between the front truck
	int m_distance;

	//Frame tail
	//SFrameTail m_frameTail;

	
};
