//
// Created by liuyu on 10/21/2017.
//

#ifndef INC_1019_LINKLAYER_H
#define INC_1019_LINKLAYER_H

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/time.h>

/*------define of macro------*/
#define WINDOWSIZE 16
#define DATALENGTH 16
#define BUFFERSIZE 16
#define MAXTIME 1  //Time to resend packet

/*------define of dataframe struct------*/
typedef struct{
    char isAck; //0->massage 1->ack 2->file
    char checksum;
    char sequenceNumber;
    char frameContent[16];
} frame;

/*------declaration of global variables------*/
int sequenceNumber=0;
int sendBase;
int nextSendNumber;
int frameNumber;//use to count the frame number
int ackNumber;
frame sendbuffer[BUFFERSIZE];
frame *pbuffer=sendbuffer;
frame recvbuf;
frame* precvbuf=&recvbuf;

//store the new ack
frame ackframe;
frame* pack=&ackframe;

/*------declaration of protocol function------*/

/*fine------create checksum------*/
char createChecksum(char *frame, int frameSize){
    int sum = 0;
    for(int i = 0; i < frameSize; i++){
        sum += (int)frame[i];
    }
    return (char)(~sum);
}

/*fine-------check checksum------*/
int checkChecksum(frame* frameget){
    char checksum=frameget->checksum;
    frameget->checksum=0;
    char checksumGet=createChecksum((char*)frameget,sizeof(frame));
    if(checksum==checksumGet){
        return 0;
    }
    else return 1;
}

/*fine------data to frame------*/
void createFrames(frame* buffer, char *data, int dataSize){
    int i=0;
    char checksum;
    for(i=0;i<dataSize;i+=16){
        strncpy(buffer[frameNumber].frameContent,data+i,16);
        buffer[frameNumber].checksum=0;
        buffer[frameNumber].isAck=0;
        buffer[frameNumber].sequenceNumber=sequenceNumber;
        buffer[frameNumber].checksum=createChecksum((char*)&buffer[frameNumber],sizeof(frame));
        printf("The frame %d is created!\n",frameNumber);
        frameNumber++;
    }
}

/*fine------frame to data------*/
int extractData(frame* recvbuf, char* content){
    strncpy(content,recvbuf->frameContent,strlen(recvbuf->frameContent));
    if(recvbuf->isAck==0)return 0;
    else return 1;
}

/*fine------Ack or Not------*/
//check whether a frame is ack or not,if it's a frame just return the ack number;
char isAck(frame* recvbuf){
    if(recvbuf->isAck==1){
        return recvbuf->sequenceNumber;
    }
    return 0;
}

/*------deal with the Ack------*/
void dealWithAck(int number){
    if(number>=sendBase){
        sendBase=ackNumber+1;
    }else{
        //ignore the wrong order ack
        return;
    }
}

/*------generate Ack------*/
void createAck(frame* ack, char ackNumber){
    bzero(ack,sizeof(ack));
    strcpy(ack->frameContent,"\0");
    ack->isAck=1;
    ack->sequenceNumber=ackNumber;
    printf("Ack %d created!\n",ackNumber);
}

/*fine------Timer------*/
//signalFunction to deal with the time out event
/*signal(SIGALRM,signalFunction);*/
void signalFunction(int sig) {
    int i;
    printf("Time out!\n");
    //resend data from sendbase to sendbase+windowsize to the physical layer
    for(i=sendBase;i<sendBase+WINDOWSIZE;i++){
        //sendToPhysical();
    }
}
//set timer
void startTimer(int frameNumber){
    printf("Timer for %d frame is start!\n",frameNumber);
    alarm(MAXTIME);
}

/*------DataLinkSend------*/
int DataLinkSend(char* data,int dataSize){
    if(nextSendNumber>sendBase+WINDOWSIZE){
        printf("The send window already full!\n");
        return 1;
    }
    else{
        createFrames(sendbuffer,data,dataSize);
        if(nextSendNumber==sendBase){
            startTimer(nextSendNumber);
        }
        //sendToPhysical()
    }
}

/*------DataLinkRecv------*/
//the return value just use to indicates whether it is a file or not
int DataLinkRecv(){

}

/*------Data from Physical layer------*/
void sendToDatalink(frame* buffer,int dataSize){
}

/*------Data send to Pyhsical layer------*/
void sendToPhysical(frame* frame,int numberOfFrame){
    //Just for test
    printf("%d frame already send to Physical Layer!\n");
}
#endif //INC_1019_LINKLAYER_H
