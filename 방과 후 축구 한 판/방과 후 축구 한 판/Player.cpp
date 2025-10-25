#include "Player.h"

// ---------------------------------------------------------------
// Player
Player::Player() = default;
Player::Player(GLfloat x, GLfloat y, GLfloat z) {
	this->position = { x, y, z };
};
Player::~Player() = default;

glm::vec3 Player::getPosition() { return this->position; };
glm::vec3 Player::getRotation() { return this->rotation; };

void Player::setPosition(GLfloat x, GLfloat y, GLfloat z) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
};

void Player::Move(Ball& ball, bool keeper_has_ball) {
	glm::vec3 distanceVec{};
	this->distance = glm::distance(glm::vec2(this->position.x, this->position.z), glm::vec2(ball.getPosition().x, ball.getPosition().z));

	if (this->has_ball) {
		if (this->sprint) {
			this->acceleration = 0.005f;
			this->max_speed = 0.1f;
			ball.setAcceleration(0.01f);
		}
		else {
			this->acceleration = 0.002f;  // �÷��̾��� ���ӵ�
			this->max_speed = 0.07f;
			ball.setAcceleration(0.002f);
		}

		// ����Ű�� ���� �÷��̾� �̵� ���� ����
		if (this->keystates[GLUT_KEY_UP]) {
			this->direction.z = -1;  // �������� �̵�
			this->rotation.y = glm::radians(180.0f);
			ball.setDirection(glm::vec3(ball.getDirection().x, ball.getDirection().y, -1.0f));
			ball.setRotation(glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		else if (this->keystates[GLUT_KEY_DOWN]) {
			this->direction.z = 1;  // �������� �̵�
			this->rotation.y = glm::radians(0.0f);
			ball.setDirection(glm::vec3(ball.getDirection().x, ball.getDirection().y, 1.0f));
			ball.setRotation(glm::vec3(1.0f, 0.0f, 0.0f)); // �ݽð���� ȸ��
		}
		else {
			this->direction.z = 0;
			ball.setDirection(glm::vec3(ball.getDirection().x, ball.getDirection().y, 0.0f));
		}
		if (this->keystates[GLUT_KEY_LEFT]) {
			this->direction.x = -1;  // �������� �̵�
			this->rotation.y = glm::radians(-90.0f);
			ball.setDirection(glm::vec3(-1.0f, ball.getDirection().y, ball.getDirection().z));
			ball.setRotation(glm::vec3(0.0f, 0.0f, 1.0f)); // �ݽð���� ȸ��
		}
		else if (this->keystates[GLUT_KEY_RIGHT]) {
			this->direction.x = 1;  // ���������� �̵�
			this->rotation.y = glm::radians(90.0f);
			ball.setDirection(glm::vec3(1.0f, ball.getDirection().y, ball.getDirection().z));
			ball.setRotation(glm::vec3(0.0f, 0.0f, -1.0f));  // 180�� ȸ��
		}
		else {
			this->direction.x = 0;
			ball.setDirection(glm::vec3(0.0f, ball.getDirection().y, ball.getDirection().z));
		}
		if (this->keystates[GLUT_KEY_UP] && this->keystates[GLUT_KEY_LEFT]) {
			this->rotation.y = glm::radians(225.0f);
		}
		else if (this->keystates[GLUT_KEY_UP] && this->keystates[GLUT_KEY_RIGHT]) {
			this->rotation.y = glm::radians(135.0f);
		}
		else if (this->keystates[GLUT_KEY_DOWN] && this->keystates[GLUT_KEY_LEFT]) {
			this->rotation.y = glm::radians(-45.0f);
		}
		else if (this->keystates[GLUT_KEY_DOWN] && this->keystates[GLUT_KEY_RIGHT]) {
			this->rotation.y = glm::radians(45.0f);
		}

		// ���ӵ��� �����ϱ� ���� �̵� ������ 0�� �ƴ��� Ȯ��
		if (glm::length(this->direction) > 0.0f) {
			// �̵� ������ ����ȭ�Ͽ� ���ӵ��� ����
			this->direction = glm::normalize(this->direction);

			// ���� �ӵ��� ���ӵ��� �����Ͽ� �ӵ� ����
			this->velocity += this->direction * this->acceleration;  // ���ӵ� ����

			// �ִ� �ӵ��� ����
			if (glm::length(this->velocity) > this->max_speed) {
				this->velocity = glm::normalize(this->velocity) * (this->max_speed);  // �ִ� �ӵ� ����
			}
		}
		else {
			// �̵����� ������ ������ ����
			if (glm::length(this->velocity) > 0.0f) {
				this->velocity -= glm::normalize(this->velocity) * (this->deceleration);  // ����
			}

			// ���� �� �ӵ��� �ʹ� �������� �ӵ��� 0���� ����
			if (glm::length(this->velocity) < min_speed) {
				this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}
	}
	else if (!this->has_ball && this-> distance <= 5.0f) {
		// �÷��̾�� �� ������ ���� ���� ���
		distanceVec = this->position - ball.getPosition();
		distanceVec = glm::normalize(distanceVec);
		// �÷��̾ ������ ���������� �ٰ������� �̵�
		this->velocity.x -= distanceVec.x * this->acceleration;
		this->velocity.z -= distanceVec.z * this->acceleration;
		ball.setAcceleration(0.0f);
	}
	else {
		if (this->keystates[GLUT_KEY_UP]) {
			this->direction.z = -1;  // �������� �̵�
			this->rotation.y = glm::radians(180.0f);
		}
		else if (this->keystates[GLUT_KEY_DOWN]) {
			this->direction.z = 1;  // �������� �̵�
			this->rotation.y = glm::radians(0.0f);
		}
		else {
			this->direction.z = 0;
		}
		if (this->keystates[GLUT_KEY_LEFT]) {
			this->direction.x = -1;  // �������� �̵�
			this->rotation.y = glm::radians(-90.0f);
		}
		else if (this->keystates[GLUT_KEY_RIGHT]) {
			this->direction.x = 1;  // ���������� �̵�
			this->rotation.y = glm::radians(90.0f);
		}
		else {
			this->direction.x = 0;
		}
		if (this->keystates[GLUT_KEY_UP] && this->keystates[GLUT_KEY_LEFT]) {
			this->rotation.y = glm::radians(225.0f);
		}
		else if (this->keystates[GLUT_KEY_UP] && this->keystates[GLUT_KEY_RIGHT]) {
			this->rotation.y = glm::radians(135.0f);
		}
		else if (this->keystates[GLUT_KEY_DOWN] && this->keystates[GLUT_KEY_LEFT]) {
			this->rotation.y = glm::radians(-45.0f);
		}
		else if (this->keystates[GLUT_KEY_DOWN] && this->keystates[GLUT_KEY_RIGHT]) {
			this->rotation.y = glm::radians(45.0f);
		}

		// ���ӵ��� �����ϱ� ���� �̵� ������ 0�� �ƴ��� Ȯ��
		if (glm::length(this->direction) > 0.0f) {
			// �̵� ������ ����ȭ�Ͽ� ���ӵ��� ����
			this->direction = glm::normalize(this->direction);

			// ���� �ӵ��� ���ӵ��� �����Ͽ� �ӵ� ����
			this->velocity += this->direction * this->acceleration;  // ���ӵ� ����

			// �ִ� �ӵ��� ����
			if (glm::length(this->velocity) > this->max_speed) {
				this->velocity = glm::normalize(this->velocity) * (this->max_speed);  // �ִ� �ӵ� ����
			}
		}
		else {
			// �̵����� ������ ������ ����
			if (glm::length(this->velocity) > 0.0f) {
				this->velocity -= glm::normalize(this->velocity) * (this->deceleration);  // ����
			}

			// ���� �� �ӵ��� �ʹ� �������� �ӵ��� 0���� ����
			if (glm::length(this->velocity) < min_speed) {
				this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// ������ ����
	if (this->position.y == 0.0f) {
		this->velocity *= FRICTION;
	}

	// �ӵ��� �������� �÷��̾� ��ġ ������Ʈ
	this->position += this->velocity;  // ���� �ӵ��� �ݿ��Ͽ� �÷��̾� ��ġ �̵�

	if (this->distance <= maxdistance && !keeper_has_ball) {
		this->has_ball = true;
	}
	else
		this->has_ball = false;
};
void Player::Draw(Ball& ball, bool keeper_has_ball, GLuint vao_player) {
	glBindVertexArray(vao_player); //--- VAO�� ���ε��ϱ�

	this->Move(ball, keeper_has_ball);
	glm::mat4 Trans = glm::mat4(1.0f);
	// �÷��̾� �̵��� ���� ��ġ ������Ʈ
	Trans = glm::translate(Trans, this->position);
	// ȸ�� ����
	Trans = glm::rotate(Trans, this->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));  // ȸ�� ����

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Trans));

	GLuint playerTextures = loadBMP("�÷��̾� ��.bmp");
	glActiveTexture(GL_TEXTURE0);      // �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, playerTextures); // �ؽ�ó ID ���

	// ���̴��� �ؽ�ó ���� 0�� ����
	GLuint texLocation = glGetUniformLocation(shaderProgramID, "Texture");
	glUniform1i(texLocation, 0);  // ���� 0�� grassTexture�� ����

	// �÷��̾� �׸���
	glDrawArrays(GL_TRIANGLES, 0, PlayerVertexCount);

	deleteTexture(playerTextures);
};

void Player::Sprint() {
	this->sprint = true;
};
void Player::Walk() {
	this->sprint = false;
};
bool Player::isSprint() { return this->sprint; };

void Player::Shoot(Ball& ball) {
	if (this->shootingInprogress && distance <= 1.5f) {
		if (this->strong)
			this->shootingpower = 100.f;
		else
			this->shootingpower += this->shooting_increase;
		if (!this->strong && this->shootingpower > this->max_shootingpower) {
			this->shootingpower = this->max_shootingpower;
		}
	}

	if (this->shootingInprogress) {
		if (ball.getPosition().y == 0.0f) {  // ���� �ٴڿ� ���� ���� �߻�
			ball.setVelocity(glm::normalize(ball.getVelocity()) * this->shootingpower);  // ���� �Ŀ� ����
			ssystem->playSound(s_shoot, 0, false, &c_shoot);

			if (this->curve) {
				ball.changeCurve();
				ball.setVelocity(glm::vec3(ball.getVelocity().x + 0.5f, this->shootingpower / 2.0f, ball.getVelocity().z));  // ��¦ ���� ƨ��� �� ���� ����
			}
			else if (this->strong) {
				ball.changeStrong();
			}
			else	
				ball.setVelocity(glm::vec3(ball.getVelocity().x, this->shootingpower / 2.0f, ball.getVelocity().z));  // ��¦ ���� ƨ��� �� ���� ����
			
			
		}
		this->shootingpower = 0.0f;  // ���� �Ŀ� �ʱ�ȭ
		this->shootingInprogress = false;  // ���� ���� �� ���� �ʱ�ȭ
	}
	this->has_ball = false;
}

bool Player::isShooting() { return this->shootingInprogress; };
void Player::changeShooting(bool shootinginprogress) {
	this->shootingInprogress = !this->shootingInprogress;
}
bool Player::isCurve() { return this->curve; };
void Player::changeCurve(bool curve) {
	this->curve = !this->curve;
}
bool Player::isStrong() { return this->strong; };
void Player::changeStrong(bool strong) {
	this->strong = !this->strong;
}

bool Player::ishasBall() { return this->has_ball; };
void Player::changehasBall(bool has_ball) {
	this->has_ball = !this->has_ball;
}

void Player::keyDown(int keys) {
	this->keystates[keys] = true;
};
void Player::keyUp(int keys) {
	this->keystates[keys] = false;
};
bool Player::isKey(int keys) { return this->keystates[keys]; };