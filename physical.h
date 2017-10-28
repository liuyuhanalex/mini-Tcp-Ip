#define PORT "5050"

typedef int (*intFuction)();
typedef void (*voidFunction)();
int createSocketServer();
int createSockerClient();

typedef struct physicalLayerStruct{
    int listener;
    int socket;
    int percLost;
    int percCorrupted;
    intFuction createSocket;
    voidFunction acceptConnection;
} PLS;

struct physicalLayerStruct createPhysicalLayer(int isServer, int lost, int corrupted){
    struct physicalLayerStruct physicalLayer;
    physicalLayer.listener = 0;
    if(isServer) {
        physicalLayer.createSocket = createSocketServer;
        physicalLayer.listener = physicalLayer.createSocket();
    } else {
        physicalLayer.createSocket = createSockerClient;
        physicalLayer.socket = physicalLayer.createSocket();
    }
    physicalLayer.percLost = lost;
    physicalLayer.percCorrupted = corrupted;
    return physicalLayer;
}

void corrupt(char *buffMsg, int sizeBuff, int percCorrupted){
    if(rand()%100>percCorrupted) return;
    if(buffMsg[0] == 'a') {
        buffMsg[0] = 'b';
    } else {
        buffMsg[0] = 'a';
    }
}

int isLost(int percLost){
    if(rand()%100 < percLost) return 1;
    return 0;
}

char *recvBuff(int socket, char *buff){
    if(recv(socket,buff,sizeof buff,0)==-1){
        perror("recv");
    }

    return buff;
}

void sendBuff(int socket, char *buff){
    if (send(socket, buff, sizeof(buff), 0) == -1) {
        perror("send");
    }
}

void acceptNewConnection(struct physicalLayerStruct physicalLayer){
    struct sockaddr_storage remoteaddr;
    int newfd;
    socklen_t addrlen;

    addrlen = sizeof remoteaddr;
    newfd = accept(physicalLayer.listener, (struct sockaddr *)&remoteaddr,  &addrlen);

    if (newfd == -1) {
        perror("accept");
    } else {
        physicalLayer.socket = newfd;
    }
}

int createSocketServer(){

    int socketfd, gai;
    int yes=1;
    struct addrinfo hints, *ai, *p;

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((gai = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(gai));
        exit(1);
    }

    for(p = ai; p != NULL; p = p->ai_next) {
        socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socketfd < 0) {
            continue;
        }
        setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(socketfd, p->ai_addr, p->ai_addrlen) < 0) {
            close(socketfd);
            continue;
        }
        break;
    }
    if (p == NULL) {
       fprintf(stderr, "selectserver: failed to bind\n");
       exit(2);
    }
    freeaddrinfo(ai);

    return socketfd;
}

int createSockerClient(){
    int socketfd, gai;
    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof memset);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((gai = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(gai));
        exit(1);
    }

    for(p = ai; p != NULL; p = p->ai_next) {
                if ((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                    perror("client: socket");
                    continue;
                }

                if (connect(socketfd, p->ai_addr, p->ai_addrlen) == -1) {
                    close(socketfd);
                    perror("client: connect");
                    continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        exit(2);
    }

    freeaddrinfo(ai);

    return socketfd;
}
