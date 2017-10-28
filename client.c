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
    PLS physicalLayer = createPhysicalLayer(0,0,0);

    char buff[50];
    strcpy(buff, "SALUT SALUT!\n");

    if (send(physicalLayer.socket, buff, sizeof(buff), 0) == -1) {
        perror("send");
    }

    return 0;
}
