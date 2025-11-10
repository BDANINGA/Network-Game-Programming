#include "include.h" 
#include "Packet.h"     
#include "function.h"    
#include "Player.h"      
#include "Ball.h"      
#include "Keeper.h"     

#define MAX_PLAYERS 3;

// --- 사용할 전역 변수 ---
PacketInputkey g_LatestInputKey[MAX_PLAYERS];
PacketInputspecialkey g_LatestInputSpecialKey[MAX_PLAYERS];

// --- 스레드에 넘겨줄 인수 ---
struct ClientContext {
    SOCKET socket;
    int playerID;
};

// --- TCP 수신 함수 ---
bool RecvTCP(SOCKET sock, char* buffer, int size) {
    int bytesRead = 0;
    while (bytesRead < size) {
        int result = recv(sock, buffer + bytesRead, size - bytesRead, MSG_WAITALL);
        if (result == SOCKET_ERROR || result == 0) {
            std::cerr << "Client disconnected (ID: " << (int)sock << ")" << std::endl;
            return false;
        }
        bytesRead += result;
    }
    return true;
}

// --- listen함수 ---
bool ListenForClients(ClientContext &context, uint16_t port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
        return false;
    }

    context.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (context.socket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return false;
    }

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);
    if (bind(context.socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0)
    {
        std::cerr << "Bind failed" << std::endl;
        closesocket(context.socket);
        WSACleanup();
        return false;
	}

    // listen()
    if (listen(context.socket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed" << std::endl;
        closesocket(context.socket);
        WSACleanup();
        return false;
	}
}

 // --- 서버 측 수신 스레드 ---
DWORD WINAPI ServerReceiveThread(LPVOID lpParam) {
    ClientContext* context = (ClientContext*)lpParam;
    SOCKET sock = context->socket;
    int playerID = context->playerID;

    PacketHeader header;
    char tempBuffer[1024]; 

    while (true) {
        // 헤더 수신
        if (!RecvTCP(sock, (char*)&header, sizeof(PacketHeader))) {
            break; 
        }

        // 패킷 정보 변환 
        header.size = ntohs(header.size);
        header.type = ntohs(header.type);

        // 실제 데이터 크기 오류 체크
        int payloadSize = header.size - sizeof(PacketHeader);
        if (payloadSize < 0 || payloadSize >(1024 - sizeof(PacketHeader))) {
            std::cerr << "잘못된 패킷 사이즈 " << playerID << std::endl;
            continue;
        }

        // 패킷 타입에 따라 분기
        switch (header.type) {

            // 입력값 처리
        case PKT_INPUT_KEY: { 
            if (!RecvTCP(sock, (char*)g_LatestInputKey[playerID].key, payloadSize)) {
                break;
            }

            break;
        }
        case PKT_INPUT_SPECIALKEY: { 
            if (!RecvTCP(sock, (char*)g_LatestInputSpecialKey[playerID].specialkey, payloadSize)) break;
            break;
        }
        
        // Login 처리
        case PKT_LOGIN: { 
            
            break;
        }

        // 3명의 플레이어가 들어왔는지 확인
        case PKT_GAME_READY: { 
            break;
        }

        default: {
            // 패킷 헤더 식별 실패
            std::cout << "Unknown packet type: " << ntohs(header.type) << std::endl;
            break;
        }

        }
    }

    // (접속 종료 처리...)
    closesocket(sock);
    delete context;
    return 0;
}



int main() {

	return 0;
}