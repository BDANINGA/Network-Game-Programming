#include "include.h" 
#include "network.h"        
#include "Player.h"           
#include "Keeper.h"     

#define MAX_PLAYERS 3
#define PORT 9000

// --- 사용할 전역 변수 ---
PacketInputkey g_LatestInputKey[MAX_PLAYERS];
PacketInputspecialkey g_LatestInputSpecialKey[MAX_PLAYERS];

int main() {

    // --- 리슨 소켓 설정 ---
    ClientContext listenContext;
    if (!ListenForClients(listenContext, PORT)) {
        return -1; // 서버 시작 실패
    }
    SOCKET listenSocket = listenContext.socket;


    // --- 클라이언트 접속 대기 (3명) ---
    SOCKET clientSockets[MAX_PLAYERS];
    HANDLE hRecvThreads[MAX_PLAYERS];

    for (int i = 0; i < MAX_PLAYERS; i++) {
        std::cout << "Waiting for player " << i << "..." << std::endl;

        // 클라이언트 접속 수락
        clientSockets[i] = AcceptClient(listenSocket);
        if (clientSockets[i] == INVALID_SOCKET) {
            i--; // 해당 클라이언트 접속 재시도
            continue;
        }
        std::cout << "Player " << i + 1 << " connected" << std::endl;

        // 수신 스레드용 컨텍스트 생성
        ClientContext* pClientContext = new ClientContext();
        pClientContext->socket = clientSockets[i];
        pClientContext->playerID = i;

        // 수신 스레드 생성
        hRecvThreads[i] = CreateServerReceiveThread(pClientContext);
        if (hRecvThreads[i] == NULL) {
            std::cerr << "Failed to create thread for player " << i << std::endl;
        }
    }

    std::cout << "All players connected. Game Start..." << std::endl;
    closesocket(listenSocket);

    // --- 메인 스레드 안에서 게임 루프 실행 ---
    //GameSessionLoop(clientSockets);

    // --- 종료 처리 ---
    std::cout << "Game loop ended. GameOver..." << std::endl;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        closesocket(clientSockets[i]);
        WaitForSingleObject(hRecvThreads[i], INFINITE);
        CloseHandle(hRecvThreads[i]);
    }

    WSACleanup();
    return 0;
}