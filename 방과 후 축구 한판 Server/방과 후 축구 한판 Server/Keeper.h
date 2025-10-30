#pragma once
#include "Player.h"

class Keeper : Object
{
public:
	Keeper();
	Keeper(GLfloat x, GLfloat y, GLfloat z);

	~Keeper();

	glm::vec3 getPosition();
	glm::vec3 getRotation();

	void setPosition(GLfloat x, GLfloat y, GLfloat z);

	void Move(glm::vec3 ballPos, bool has_ball);
	void Draw(glm::vec3 ballPos, bool has_ball, GLuint vao_player);

	void hasBall();
	void NothasBall();
	bool ishasBall();

private:
	float minX = -3.0f;
	float maxX = 3.0f;
	float minY = -2.0f;
	float maxY = 2.0f;
	float targetX = 0;
	float targetY = -0.1f;

	float velocity = 0.0125f; // ��Ű���� �̵� �ӵ�


	bool has_ball = false;
};

