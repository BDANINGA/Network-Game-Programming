#pragma once
#include "Ball.h"

class Player : Object
{
public:
	Player();
	Player(GLfloat x, GLfloat y, GLfloat z);

	~Player();

	glm::vec3 getPosition();
	glm::vec3 getRotation();

	void setPosition(GLfloat x, GLfloat y, GLfloat z);

	void Move(Ball& ball, bool keeper_has_ball);
	void Draw(Ball& ball, bool keeper_has_ball, GLuint vao_player);

	void Sprint();
	void Walk();
	bool isSprint();

	void Shoot(Ball& ball);

	bool isShooting();
	void changeShooting(bool shootinginprogress);
	bool isCurve();
	void changeCurve(bool curve);
	bool isStrong();
	void changeStrong(bool strong);

	bool ishasBall();
	void changehasBall(bool has_ball);

	void keyDown(int keys);
	void keyUp(int keys);
	bool isKey(int keys);



private:
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);  // �ӵ�
	glm::vec3 direction{};  // �̵� ����
	float acceleration = 0.05f;  // �÷��̾��� ���ӵ�

	float max_speed = 10.0f; // �ִ� �ӵ�
	const float deceleration = 0.01f; // ���� (���ӵ��� �ݴ�)
	
	float distance{};	// ���� �÷��̾� ������ �Ÿ�
	float shootingpower{};  // ���� �Ŀ� ���� (0.0 ~ MAX_SHOOTING_POWER ����)
	const float max_shootingpower = 50.0f;  // �ִ� ���� �Ŀ�
	const float shooting_increase = 1.0f;  // ���� �Ŀ� ������

	bool sprint{};
	bool has_ball= true;	// ù �������� ���� true�� �ٲ�
	bool keystates[256]{};	// Ű���� ����
	bool shootingInprogress{};  // ���� ���� �� ���� (d Ű�� ���� ��������)
	bool curve = false;			// ��������
	bool strong = false;		// �Ŀ���

};