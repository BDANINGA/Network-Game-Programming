#include "Keeper.h"

// ---------------------------------------------------------------
// Keeper
Keeper::Keeper() = default;
Keeper::Keeper(GLfloat x, GLfloat y, GLfloat z) {
	this->position = { x, y, z };
};
Keeper::~Keeper() = default;

glm::vec3 Keeper::getPosition() { return this->position; };
glm::vec3 Keeper::getRotation() { return this->rotation; };

void Keeper::Move(glm::vec3 ballPos, bool has_ball) {
	// ��Ű���� �̵� ���� ����
	this->targetX = ballPos.x;
	if (!has_ball)
	{
		this->velocity = 0.05f;
		//targetY = ballPos.y;
	}

	else {
		this->targetY = -0.1f;
		this->minX = -1.0f;
		this->maxX = 1.0f;
	}
	// ��ǥ ��ġ�� ���� ���� ����
	this->targetX = glm::clamp(this->targetX, this->minX, this->maxX);
	this->targetY = glm::clamp(this->targetY, this->minY, this->maxY);

	// ��Ű�� ��ġ�� ��ǥ ��ġ�� �ε巴�� �̵� (lerp ��� ���)
	this->position.x = glm::mix(this->position.x, this->targetX, this->velocity);
	if (ballPos.z - this->position.z <= 5)
		this->position.y = glm::mix(this->position.y, this->targetY, this->velocity);
};
void Keeper::Draw(glm::vec3 ballPos, bool has_ball, GLuint vao_player) {
	glBindVertexArray(vao_player); //--- VAO�� ���ε��ϱ�

	// move�� �����ʰ� Server�κ��� recv�� position�� �޴´�.
	// this->Move(ballPos, has_ball);

	glm::mat4 Trans = glm::mat4(1.0f);
	// �÷��̾� �̵��� ���� ��ġ ������Ʈ
	Trans = glm::translate(Trans, this->position);

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Trans));

	GLuint keeperTextures = loadBMP("�÷��̾� ��.bmp");
	glActiveTexture(GL_TEXTURE0);      // �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, keeperTextures); // �ؽ�ó ID ���

	// ���̴��� �ؽ�ó ���� 0�� ����
	GLuint texLocation = glGetUniformLocation(shaderProgramID, "Texture");
	glUniform1i(texLocation, 0);  // ���� 0�� grassTexture�� ����

	// �÷��̾� �׸���
	glDrawArrays(GL_TRIANGLES, 0, PlayerVertexCount);

	deleteTexture(keeperTextures);
};

void Keeper::hasBall() {
	this->has_ball = true;
};
void Keeper::NothasBall() {
	this->has_ball = false;
};
bool Keeper::ishasBall() { return this->has_ball; };