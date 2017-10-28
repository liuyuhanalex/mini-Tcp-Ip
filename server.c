#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "physical.h"

int main(){
    PLS physicalLayer = createPhysicalLayer(1,0,0);
    char buff[50];

    while(1){
        acceptNewConnection(physicalLayer);
        recvBuff(physicalLayer.socket, buff);
        printf("%s\n", buff);
    }

    return 0;
}
