#include "MessageHandler.h"
#include "TransmissionHandler.h"
#include "SFollowingTruckInfo.h"
#include "SMessageFeedBack.h"

#include <iostream>

int main()
{
    TransmissionHandler transHandler; // create TransmissionHandler object
    transHandler.NetworkConfiguration(); // configure network settings
    transHandler.ThreadInitialization();// initialize TransmissionHandler thread

    MessageHandler msgHandler;// create MessageHandler object
    msgHandler.ThreadInitialization();// initialize MessageHandler thread

    while (1) // infinite loop to continuously receive and handle messages
    {
        SFollowingTruckInfo msg = transHandler.recvMsg(); // receive message from TransmissionHandler

        msgHandler.OnMessage(msg, transHandler.clientAddr()); // pass message and client address to MessageHandler

        Sleep(500); // sleep for 500 milliseconds

        SMessageFeedBack feedback = msgHandler.getHandlingFeedBack(); // retrieve feedback from MessageHandler

        transHandler.sendMsgOnce(feedback); // send feedback message to TransmissionHandler
    }
}

