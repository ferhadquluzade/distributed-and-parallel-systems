#pragma once

enum RequestType
{
	CONTROL_REQUEST = 0,
	JOIN_REQUEST,
	LEAVE_REQUEST

};

enum Permission
{
	REFUSED = 0,
	ALLOWED
	
};

enum FeedBackType
{
	CONTROL_FEEDBACK = 0,
	JOIN_FEEDBACK,
	LEAVE_FEEDBACK
};