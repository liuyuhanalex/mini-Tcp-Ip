typedef void (*sendFunction);
typedef void (*recvFunction);
void sendBuff();
char *recvBuff();

typedef struct LinkLayerStruct {
    sendFunction sendf;
    recvFunction recvf;
} LLS;

struct LinkLayerStruct createLinkLayer(){
    struct LinkLayerStruct linkLayer;
    linkLayer.sendf = sendBuf;
    linkLayer.recvf = recvBuff;
}

void sendBuff(){

}

char *recvBuff(char *buff){
    return buff;
}
