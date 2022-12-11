// udp client driver program
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include <thread>
#define PORT 11500
#define MAXLINE 1000
#define SENDER 379 //Sender truck id example
#define RECV 110 //Receiver truck id example

void stop(){
    printf("stopping...");
}

void speed_up(){
    printf("speeding up...");
}

void speed_down(){
    printf("speeding down...");
}

typedef enum flags{
    
    SPEED_DOWN = -2,
    FAILED=-1,
    STOP = 0,
    READY = 1,
    SPEED_UP = 2
} Flags;

typedef struct Data{
    int sender_id;
    int recv_id;
    Flags flag;
    std::string comment;
    Data(int sender, int recv, Flags flag, std::string comm=" "){
        sender_id = sender;
        recv_id = recv;
        flag = flag;
        comment = comm;
    }
} Data;

// Driver code
int main()
{
    Flags flag = READY;
    printf("started");
    char buffer[100];
    Data data(SENDER,RECV, flag);
    std::string message = "hello world";
    int sockfd, n;
    struct sockaddr_in servaddr;
    
    // clear servaddr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr("10.214.24.254");
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;
    
    // create datagram socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // connect to server
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }
    
    // request to send datagram
    // no need to specify server address in sendto
    // connect stores the peers IP and port
    sendto(sockfd, &data, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr));
    while(1){
        int msgLen = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
        sleep(1);
        //        ====================================================================================
        //required code for new version(from my point if view, but i could not).
        //        switch(buffer){
        //            .......
        speed_up();
        speed_down();
        stop();
        //        }
        
        
        //Previous code implemented in last dicussion.............
        //        if(strcmp(buffer, "speed up")==0)
        //            printf("speeding up");
        //        else{
        //            printf("error...");
        //        }
        //        ====================================================================================
    }
    
    // waiting for response
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
    puts(buffer);
    
    // close the descriptor
    close(sockfd);
}
