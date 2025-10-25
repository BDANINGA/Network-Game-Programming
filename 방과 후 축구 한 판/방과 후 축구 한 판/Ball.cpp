#include "Ball.h"

// ---------------------------------------------------------------
// Ball
Ball::Ball() {
	this->rotation = glm::vec3(0.1f, 0.1f, 0.1f);
};
Ball::Ball(GLfloat x, GLfloat y, GLfloat z) {
	this->position = { x, y, z };
	this->rotation = glm::vec3(0.1f, 0.1f, 0.1f);
};
Ball::~Ball() = default;

glm::vec3 Ball::getPosition() { return this->position; };
glm::vec3 Ball::getRotation() { return this->rotation; };
glm::vec3 Ball::getVelocity() { return this->velocity; };
glm::vec3 Ball::getDirection() { return this->direction; };
float Ball::getAcceleration() { return this->acceleration; };

void Ball::setPosition(GLfloat x, GLfloat y, GLfloat z) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
};
void Ball::setRotation(glm::vec3 rotation) {
	this->rotation = rotation;
};
void Ball::setVelocity(glm::vec3 velocity) {
	this->velocity = velocity;
};
void Ball::setDirection(glm::vec3 direction) {
	this->direction = direction;
}
void Ball::setAcceleration(float acceleration) {
	this->acceleration = acceleration;
}

void Ball::Move(glm::vec3 keeperPos, bool keeper_has_ball) {
	// ���� �ӵ� ������Ʈ
	this->velocity += this->acceleration * this->direction;

	// ����� �� ��ǰ ��ǥ �� ũ��
	glm::vec3 goalBarPos = glm::vec3(0.0f, 2.0f, -35.0f);
	glm::vec3 goalBarScale = glm::vec3(2.0f, 0.05f, 1.0f);

	glm::vec3 leftPostPos = glm::vec3(-2.0f, 1.0f, -35.0f);
	glm::vec3 leftPostScale = glm::vec3(0.05f, 1.0f, 1.0f);

	glm::vec3 rightPostPos = glm::vec3(2.0f, 1.0f, -35.0f);
	glm::vec3 rightPostScale = glm::vec3(0.05f, 1.0f, 1.0f);

	glm::vec3 bottomBarPos = glm::vec3(0.0f, 1.0f, -36.0f);
	glm::vec3 bottomBarScale = glm::vec3(2.0f, 1.0f, 0.05f);

	glm::vec3 startPos = this->position;
	glm::vec3 endPos = this->position + this->velocity; // ���� �̵� ����

	// **��Ű�ۿ��� �浹 ó��**
	glm::vec3 keeperSize = glm::vec3(0.5f, 0.7f, 0.5f);  // ��Ű�� ���� ũ�� ����
	//std::cout << keeperPos.x - keeperSize.x << std::endl;
	// �浹 üũ: ���� ��Ű�۰� �浹�ߴ��� �˻�
	if (this->position.x > keeperPos.x - keeperSize.x && this->position.x < keeperPos.x + keeperSize.x &&
		this->position.z > keeperPos.z - keeperSize.z && this->position.z < keeperPos.z + keeperSize.z &&
		this->position.y > keeperPos.y - keeperSize.y && this->position.y < keeperPos.y + keeperSize.y) {
		// ���� ��Ű�� ����� �浹�ߴٸ�
		std::cout << "��Ű�ۿ� �浹!" << std::endl;
		keeper_has_ball = 1;
		this->velocity *= 0.0f;
	}
	else
	{
		keeper_has_ball = 0;
	}
	// ������ �浹 ó��
	if (
		(checkSegmentCollision(startPos, endPos, rightPostPos, rightPostScale) && this->position.z <= -32) ||
		(checkSegmentCollision(startPos, endPos, leftPostPos, leftPostScale) && this->position.z <= -32) ||
		(checkSegmentCollision(startPos, endPos, goalBarPos, goalBarScale) && this->position.z <= -35) ||
		(checkSegmentCollision(startPos, endPos, bottomBarPos, bottomBarScale) && this->position.z <= -35)
		)
	{
		// �浹 ó��
		this->velocity = -this->velocity * BALL_BOUNCE_DAMPING;
		std::cout << "�浹 ���" << std::endl;

		// ��뿡 ��
		if (checkSegmentCollision(startPos, endPos, bottomBarPos, bottomBarScale) && this->position.z <= -35) {
			std::cout << "��" << std::endl;
			ssystem->playSound(s_goal, 0, false, &c_goal);

			this->position = glm::vec3(0.0f, 0.0f, 0.0f);
			this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}
	// �߷� ����
	this->velocity.y += GRAVITY;

	// �ִ� �ӵ� ����
	float speed = glm::length(this->velocity);
	if (speed > max_speed) {
		this->velocity = glm::normalize(this->velocity) * max_speed;
	}

	// ������ ����
	if (this->position.y == 0.0f) {
		this->velocity *= FRICTION;
	}

	// ���� ���ߵ���: ���� �ӵ� ���Ϸ� �������� ���� ����
	if (glm::length(this->velocity) < min_speed) {
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotationAngle = 0.0f;  // ���� ���߸� ȸ���� ����
	}
	if (this->curve)
		this->velocity.x -= this->CURVE_TURN_SPEED;
	if (this->strong)
		this->velocity.z += 0.5f;


	// ���� ��ġ ������Ʈ
	this->position += this->velocity;

	// ���� �ٴڿ� ������ �ݻ�
	if (this->position.y < 0.0f) {
		this->position.y = 0.0f;
		this->velocity.y = -this->velocity.y * BALL_BOUNCE_DAMPING;

		this->curve = false;
		this->strong = false;
		// ���� ���Ϸ� �������� ����
		if (this->velocity.y < 0.2f)
			this->velocity.y = 0;
	}
	

	// x�� z ��迡 ������ �ݻ� ó��
	if (this->position.x < -50.0f) {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotationAngle = 0.0f;  // ���� ���߸� ȸ���� ����
	}
	else if (this->position.x > 50.0f) {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotationAngle = 0.0f;  // ���� ���߸� ȸ���� ����
	}

	if (this->position.z < -50.0f) {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotationAngle = 0.0f;  // ���� ���߸� ȸ���� ����
	}
	else if (this->position.z > 50.0f) {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotationAngle = 0.0f;  // ���� ���߸� ȸ���� ����
	}

	// ȸ�� ���� ������Ʈ (���� ���� ���⿡ ���)
	this->rotationAngle += glm::length(this->velocity) * rotationSpeed;

};
void::Ball::Draw(glm::vec3 keeperPos, bool keeper_has_ball, GLuint vao_ball) {
	this->Move(keeperPos, keeper_has_ball);
	glBindVertexArray(vao_ball); //--- VAO�� ���ε��ϱ�

	glm::mat4 T = glm::mat4(1.0f);
	glm::mat4 S = glm::mat4(1.0f);
	glm::mat4 R = glm::mat4(1.0f);  // ȸ�� ���
	glm::mat4 Trans = glm::mat4(1.0f);

	// ȸ�� ���� (ȸ�� ���� ���� ����)
	R = glm::rotate(R, this->rotationAngle, this->rotation);  // ���⿡ ���� ȸ��

	// �� ��° ��ü(��) �̵��� ���� ��ġ ������Ʈ
	T = glm::translate(T, this->position);  // �� ��° ��ü�� ��ġ ����
	S = glm::scale(S, glm::vec3(0.015f, 0.015f, 0.015f));  // ũ�� ����
	Trans = T * R * S; // ��ġ, ȸ��, ũ�� ������ ����

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Trans));

	GLuint ballTextures = loadBMP("�౸��.bmp");
	glActiveTexture(GL_TEXTURE0);      // �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, ballTextures); // �ؽ�ó ID ���

	// ���̴��� �ؽ�ó ���� 0�� ����
	GLuint texLocation = glGetUniformLocation(shaderProgramID, "Texture");
	glUniform1i(texLocation, 0);  // ���� 0�� grassTexture�� ����

	// �� ��° ��ü(��) �׸���
	glDrawArrays(GL_TRIANGLES, 0, BallVertexCount);

	deleteTexture(ballTextures);
};

void Ball::changeCurve() {
	this->curve = !this->curve;
}
void Ball::changeStrong() {
	this->strong = !this->strong;
}