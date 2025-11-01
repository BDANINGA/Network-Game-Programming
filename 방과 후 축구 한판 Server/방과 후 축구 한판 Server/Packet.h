#pragma once
#include "include.h"
#pragma pack(push, 1)
#define PKT_LOGIN_RESULT 100
#define PKT_USER_DATA 101
#define PKT_RENDER_DATA 102
#define PKT_GAMEOVER 103
#define PKT_INPUT_KEY 104
#define PKT_INPUT_SPECIALKEY 105
#define PKT_LOGIN 106
#define PKT_GAME_READY 107
#define PKT_CHAT_MESSAGE 108

struct PacketHeader {
	uint16_t type;    // ��Ŷ ����(ex.�α���, ����������, Ű�Է� ��)
	uint16_t size;    // ��ü ��Ŷ ũ��
};

// Server -> Client
struct PacketLoginResult {
	PacketHeader header{ htons(PKT_LOGIN_RESULT), htons(sizeof(PacketLoginResult)) };
	uint8_t success;
	char message[64];      // ���� �� ���� or ���� �޽���
};

struct UserData {
	int totalMatch;
	int win;
	int lose;
	float winRate;
};

struct PacketUserData {
	PacketHeader header{ htons(PKT_USER_DATA), htons(sizeof(PacketUserData)) };
	UserData data;
};

struct PlayerData {
	int id;
	glm::vec3 position;
	glm::vec3 rotation;
};

struct BallData {
	glm::vec3 position;
	glm::vec3 rotation;
	float rotationAngle;
};

struct KeeperData {
	glm::vec3 position;
	glm::vec3 rotation;
};

struct PacketRenderData {
	PacketHeader header{ htons(PKT_RENDER_DATA), htons(sizeof(PacketRenderData)) };
	PlayerData p_data;
	BallData b_data;
	KeeperData k_data;
};

struct PacketGameover {
	PacketHeader header{ htons(PKT_GAMEOVER), htons(sizeof(PacketGameover)) };
	uint8_t gameover;
};

// Client -> Server
struct PacketInputkey {
	PacketHeader header{ htons(PKT_INPUT_KEY), htons(sizeof(PacketInputkey)) };
	uint8_t key[256];
};

struct PacketInputspecialkey {
	PacketHeader header{ htons(PKT_INPUT_SPECIALKEY), htons(sizeof(PacketInputspecialkey)) };
	uint8_t specialkey[256];
};

struct PacketLogin {
	PacketHeader header{ htons(PKT_LOGIN), htons(sizeof(PacketLogin)) };
	char userID[32];
	char userPW[32];
};

struct PacketGameReady {
	PacketHeader header{ htons(PKT_GAME_READY), htons(sizeof(PacketGameReady)) };
	uint8_t ready;
};

struct PacketChatMessage {
	PacketHeader header{ htons(PKT_CHAT_MESSAGE), htons(sizeof(PacketChatMessage)) };
	char message[256];
};