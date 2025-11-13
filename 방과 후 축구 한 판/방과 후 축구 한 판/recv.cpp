#include "network.h"

// recv_gameover() - 11.08
void recv_gameover(SOCKET socket, PacketHeader header, bool* gameover) {
    int received = recv(socket, (char*)gameover, header.size, MSG_WAITALL);
    if (received == SOCKET_ERROR)
        perror("recv_gameover");
}
