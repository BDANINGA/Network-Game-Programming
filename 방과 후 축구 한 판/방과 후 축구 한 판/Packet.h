#pragma once
#include "include.h"

struct PacketHeader {
	uint16_t type;    // 패킷 종류(ex.로그인, 유저데이터, 키입력 등)
	uint16_t size;    // 전체 패킷 크기
};

// Server -> Client
struct PacketLoginResult {
	PacketHeader header;
	uint8_t success;
	char message[64];      // 실패 시 이유 or 성공 메시지
};

struct UserData {
	int totalMatch;
	int win;
	int lose;
	float winRate;
};

struct PacketUserData {
	PacketHeader header;
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
	PacketHeader header;
	PlayerData p_data;
	BallData b_data;
	KeeperData k_data;
};

struct PacketGameover {
	PacketHeader header;
	uint8_t gameover;
};

// Client -> Server
struct PacketInputkey {
	PacketHeader header;
	uint32_t key;
};

struct PacketInputspeicialkey {
	PacketHeader header;
	uint32_t specialkey;
};

struct PacketLogin {
	PacketHeader header;
	char userID[32];
	char userPW[32];
};

struct PacketGameReady {
	PacketHeader header;
	uint8_t ready;
};

struct PacketChatMessage {
	PacketHeader header;
	std::string Message;
};


