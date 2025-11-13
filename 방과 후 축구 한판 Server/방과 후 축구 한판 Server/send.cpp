#include "network.h"

// send_gameover - 11.8일 구현
void send_gameover(SOCKET socket) {
    PacketGameover gameover;
    gameover.gameover = true;
    int sent1 = send(socket, (char*)&gameover.header, sizeof(PacketHeader), 0);
    if (sent1 == sizeof(PacketHeader))
        printf("error send_gameover.header");

    int sent2 = send(socket, (char*)&gameover.gameover, gameover.header.size, 0);    // 제대로 동작하는가? -> 구조체 정의가 완전히 동일, Padding 없음, 서버와 클라는 같은 플랫폼, 엔디안이 같음.
    if (sent2 == sizeof(gameover.header.size))
        printf("error send_gameover.gameover");
}