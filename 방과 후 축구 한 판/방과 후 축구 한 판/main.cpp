//--- 메인 함수
#include "방과후 축구한판_Client.h"

// --- 네트워크 통신용 전역 변수 ---
SOCKET g_ServerSocket;

// --- send 용도 ---
uint8_t g_currentKey[256] = { 0 };
uint8_t g_currentSpecialKey[256] = { 0 };

// --- recv 용도 ---
bool gameover{};

// --- 수신된 패킷을 처리하는 함수 ---
void ProcessPacket(SOCKET socket, const PacketHeader& header, char* payload)
{
    uint16_t type = ntohs(header.type);

    switch (type)
    {
    case PKT_RENDER_DATA:
    {
        memcpy(&g_LatestRenderData, &header, sizeof(PacketHeader));
        memcpy(((char*)&g_LatestRenderData) + sizeof(PacketHeader),
            payload, sizeof(PacketRenderData) - sizeof(PacketHeader));
        break;
    }
    case PKT_LOGIN_RESULT:
    {
        memcpy(&g_LatestLoginResult, &header, sizeof(PacketHeader));
        memcpy(((char*)&g_LatestLoginResult) + sizeof(PacketHeader),
            payload, sizeof(PacketLoginResult) - sizeof(PacketHeader));
        break;
    }
    case PKT_GAMEOVER:                                  // 이런 식으로 처리?
    {
        recv_gameover(socket, header, &gameover);
        break;
    }
    default:
        std::cout << "Unknown packet type: " << type << std::endl;
        break;
    }
}


// --- 클라이언트 네트워크 스레드 ---
DWORD WINAPI ClientNetworkThread(LPVOID lpParam)
{
    SOCKET sock = (SOCKET)lpParam;

    while (true)
    {
        // =====================
        // (1) 클라이언트 입력 전송
        // =====================
        PacketInputkey keyPkt;
        PacketInputspecialkey specialPkt;

        memcpy(keyPkt.key, g_currentKey, sizeof(g_currentKey));
        memcpy(specialPkt.specialkey, g_currentSpecialKey, sizeof(g_currentSpecialKey));

        send(sock, (char*)&keyPkt, sizeof(keyPkt), 0);
        send(sock, (char*)&specialPkt, sizeof(specialPkt), 0);

        // =====================
        // (2) 패킷 수신
        // =====================
        PacketHeader header;
        int bytesReceived = 0;
        int totalReceived = 0;

        // --- 헤더(4바이트) 수신 ---
        while (totalReceived < sizeof(PacketHeader))
        {
            bytesReceived = recv(sock, ((char*)&header) + totalReceived,
                sizeof(PacketHeader) - totalReceived, 0);
            if (bytesReceived <= 0)
            {
                std::cerr << "서버 연결 종료" << std::endl;
                closesocket(sock);
                return 0;
            }
            totalReceived += bytesReceived;
        }

        uint16_t packetSize = ntohs(header.size);
        int payloadSize = packetSize - sizeof(PacketHeader);

        // --- 페이로드 수신 ---
        char payloadBuffer[2048];
        totalReceived = 0;
        while (totalReceived < payloadSize)
        {
            bytesReceived = recv(sock, payloadBuffer + totalReceived,
                payloadSize - totalReceived, 0);
            if (bytesReceived <= 0)
            {
                std::cerr << "서버 연결 종료" << std::endl;
                closesocket(sock);
                return 0;
            }
            totalReceived += bytesReceived;
        }

        // =====================
        // (3) 패킷 처리
        // =====================
        ProcessPacket(sock, header, payloadBuffer);
    }

    closesocket(sock);
    return 0;
}

//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

// 바인드 공간
std::vector<GLfloat> vertexArray[3];
std::vector<GLfloat> colorArray[3];
std::vector<GLfloat> normalArray[3];
std::vector<GLfloat> textureArray[3];

// 바인드
GLuint vao_player, vbo_player[4], vao_ball, vbo_ball[4], vao_goalpost, vbo_goalpost[4];

// 버텍스 길이
int PlayerVertexCount{}, BallVertexCount{}, GoalPostVertexCount{};

//사운드
FMOD::System* ssystem;
FMOD::Sound* s_bgm, * s_goal, * s_touch, * s_shoot;
FMOD::Channel* c_bgm = 0;
FMOD::Channel* c_goal = 0;
FMOD::Channel* c_touch = 0;
FMOD::Channel* c_shoot = 0;
FMOD_RESULT result;
void* extradriverdata = 0;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	srand(time(NULL));
	width = 1200;
	height = 800;
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	//--- 세이더 읽어와서 세이더 프로그램 만들기
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	shaderProgramID = make_shaderProgram(); //--- 세이더 프로그램 만들기

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(SpecialKeys);       // 방향키 입력 콜백 함수
	glutSpecialUpFunc(SpecialKeysUp);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}






