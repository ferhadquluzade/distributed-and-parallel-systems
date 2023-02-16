#include "MessageHandler.h"
#include "TransmissionHandler.h"
#include "SFollowingTruckInfo.h"
#include "SMessageFeedBack.h"

#include <iostream>

int main()
{
    TransmissionHandler transHandler;
    transHandler.NetworkConfiguration();
    transHandler.ThreadInitialization();

    MessageHandler msgHandler;
    msgHandler.ThreadInitialization();

    while (1)
    {
        SFollowingTruckInfo msg = transHandler.recvMsg();

        msgHandler.OnMessage(msg, transHandler.clientAddr());

        Sleep(500);

        SMessageFeedBack feedback = msgHandler.getHandlingFeedBack();

        transHandler.sendMsgOnce(feedback);
    }
}

