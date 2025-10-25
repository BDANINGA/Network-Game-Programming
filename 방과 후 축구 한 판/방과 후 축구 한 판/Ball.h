#pragma once
#include "Object.h"

const float maxdistance = 0.75f;  // ���� �÷��̾� ������ �ִ� �Ÿ�
const float min_speed = 0.01f; // ���� �ӵ� �Ѱ谪

class Ball : Object
{

public:
	Ball();
	Ball(GLfloat x, GLfloat y, GLfloat z);

	~Ball();

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getVelocity();
	glm::vec3 getDirection();
	float getAcceleration();

	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setRotation(glm::vec3 rotation);
	void setVelocity(glm::vec3 velocity);
	void setDirection(glm::vec3 direction);
	void setAcceleration(float acceleration);

	void Move(glm::vec3 keeperPos, bool keeper_has_ball);
	void Draw(glm::vec3 keeperPos, bool keeper_has_ball, GLuint vao_ball);

	void changeCurve();
	void changeStrong();

private:
	glm::vec3 velocity{};
	glm::vec3 direction{};
	float acceleration{};
	const float max_speed = 1.0f; // ���� �ִ� �ӵ�
	
	// ���� ȸ�� ������ ȸ�� �ӵ� ���� �߰�
	float rotationAngle{};  // ȸ�� ���� (����)
	const float rotationSpeed = 5.0f;  // ���� ȸ�� �ӵ� (����: ����/��)
	const float CURVE_TURN_SPEED = 0.03f; // �������� ȸ����

	bool curve{};
	bool strong{};
};