#pragma once
#include "function.h"

const float FRICTION = 0.98f; // ������ (�ӵ� ���� ����)
const float GRAVITY = -0.05f;  // �߷� ��, ������ �����Ͽ� �Ʒ��� ����������
const float BALL_BOUNCE_DAMPING = 0.8f;  // �ٿ ���� (0~1 ���̷� ����, 1�̸� �ݻ� �� �ӵ� �״��)

class Object
{
protected:
	glm::vec3 position{};
	glm::vec3 rotation{};
	float rotationAngle{};

public:
	Object();
	glm::vec3 getPosition();
};

void drawGoal(GLuint vao_goalpost);
void drawBackground();