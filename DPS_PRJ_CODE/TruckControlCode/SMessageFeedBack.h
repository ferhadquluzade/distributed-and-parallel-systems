#pragma once

struct SMessageFeedBack
{
	//0 control feedback; 1 join feedback; 2 leave feedback
	short m_FeedbackType;

	//0 not allowed; 1 allowed
	short m_JoinAllowed;

	//0 not allowed; 1 allowed
	short m_LeaveAllowed;

	short m_sequenceNo;
	/*Control Instruction*/
	//0 Brake; 1 Start; 2 Speed up; 3 Speed down;
	short m_movingInstruction;

	int m_velocity;

	SMessageFeedBack():
		m_JoinAllowed(0),
		m_LeaveAllowed(0),
		m_movingInstruction(0),
		m_velocity(0)
	{}
};
