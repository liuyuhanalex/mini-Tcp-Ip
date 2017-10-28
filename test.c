//
// Created by liuyu on 10/24/2017.
//

#include "linklayer.h"

int main(void){
    char content[16]="0";
    signal(SIGALRM,signalFunction);
    char test[]="1234568liuyuhan";

    DataLinkSend(test,sizeof(test));
    printf("the content of [0] is:%s\n",pbuffer[0].frameContent);

    int file=extractData(&pbuffer[0],content);
    printf("the content extra is:%s\n",content);
    printf("the file number is %d\n",file);

    printf("the checksum is %d\n",pbuffer[0].checksum);

    printf("is that corrupt:%d\n",checkChecksum(&pbuffer[0]));
    pbuffer[0].checksum=createChecksum((char*)&pbuffer[0],sizeof(frame));
    //strcpy(pbuffer[0].frameContent,"lalalla");

    printf("is that corrupt:%d\n",checkChecksum(&pbuffer[0]));
    createAck(pack,2);
    printf("is ack or not:%d\n",isAck(&pbuffer[0]));
    printf("is ack or not(the ack number):%d\n",isAck(pack));

    while(1);
}