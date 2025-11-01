#pragma once
#include "include.h"

struct PacketHeader {
	uint16_t type;    // ��Ŷ ����(ex.�α���, ����������, Ű�Է� ��)
	uint16_t size;    // ��ü ��Ŷ ũ��
};

// Server -> Client
struct PacketLoginResult {
	PacketHeader header;
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


