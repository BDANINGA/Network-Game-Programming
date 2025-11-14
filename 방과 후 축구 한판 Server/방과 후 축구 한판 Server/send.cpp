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

void send_renderdata(SOCKET socket, PacketRenderData& renderdata) {
    int sent1 = send(socket, (char*)&renderdata.header, sizeof(PacketHeader), 0);
    if (sent1 == sizeof(PacketHeader))
        printf("error send_renderdata.header");

    int sent2 = send(socket, (char*)&renderdata.p_data, sizeof(PlayerData), 0);
    if (sent2 == sizeof(PlayerData))
        printf("error send_gameover.p_data");

    int sent3 = send(socket, (char*)&renderdata.b_data, sizeof(BallData), 0);
    if (sent3 == sizeof(BallData))
        printf("error send_gameover.b_data");

    int sent4 = send(socket, (char*)&renderdata.k_data, sizeof(KeeperData), 0);
    if (sent4 == sizeof(KeeperData))
        printf("error send_gameover.k_data");

}