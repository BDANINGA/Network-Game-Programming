#define _CRT_SECURE_NO_WARNINGS
//--- ���� �Լ�
#pragma comment(lib,"glew32.lib") 
#pragma comment(lib,"freeglut.lib") 
#include "����� �౸����_Client.h"

//------------------------------------------------------------------------------------------------------
Player player;
Ball ball;
Keeper keeper(0.0f, 0.0f, -32.0f);

Camera camera;
Light light;
//------------------------------------------------------------------------
bool start = true;
bool left_button = 0;
// ������
GLvoid drawScene() {
	
	if (start) {
		start = false;
		loadObj();
		InitBuffer();
	}

	//--- ����� ���� ����
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);
	glEnable(GL_DEPTH_TEST);   // ���� �׽�Ʈ Ȱ��ȭ
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// server�κ��� �����͸� �޾Ƽ� ����Ϸ��� ���ڸ� �ٲ�� ��.
	// ball.Draw(ballPos, ,ballR, ballR_Angle);
	// player.Draw(playerPos, playerR);
	// keeper.Draw(keeperPos);
	ball.Draw(keeper.getPosition(), keeper.ishasBall(), vao_ball);						// server�κ��� position, rotationAngle, rotation�� ������ �ȴ�.
	player.Draw(ball, keeper.ishasBall(), vao_player);									// server�κ��� position, rotation�� ������ �ȴ�.
	keeper.Draw(ball.getPosition(), player.ishasBall(), vao_player);					// server�κ��� position�� ������ �ȴ�.

	drawGoal(vao_goalpost);
	drawBackground();

	// ī�޶� ����: �÷��̾ ���󰡴� ī�޶�
	camera.setPosition(player.getPosition() + glm::vec3(0.0f, 1.0f, 5.0f));  // �÷��̾� ��ġ �������� ī�޶� ��ġ ���� (�� 2, �� 5)
	camera.setDirection(player.getPosition());  // ī�޶�� �÷��̾ ���ϵ��� ����

	viewTransform();
	projectionTransform();
	make_Light();

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}


// ����
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {

	case 'd':
	case 'D':
		// d Ű�� ��ҹ��ڷ� ó��, ��ҹ��� ����
		player.changeShooting(player.isShooting());
		break;

	case 'r':
	case 'R':
		ball.setPosition(player.getPosition().x, player.getPosition().y, player.getPosition().z);
		ball.setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		break;
	case 'e':
	case 'E':
		player.Sprint();
		break;
	case 'z':
	case 'Z':
		player.changeCurve(player.isCurve());
		break;
	case 'c':
	case 'C':
		player.changeStrong(player.isStrong());
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}
void KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'd':
	case 'D':
		// d Ű�� �������� �� �߻�
		player.Shoot(ball);
		break;
	case 'e':
	case 'E':
		player.Walk();
		break;
	case 'z':
	case 'Z':
		player.changeCurve(player.isCurve());
		break;
	case 'c':
	case 'C':
		player.changeStrong(player.isStrong());
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}
void Mouse(int button, int state, int x, int y)
{
	float gl_x, gl_y;
	windowToOpenGL(x, y, width, height, gl_x, gl_y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
}
void Motion(int x, int y)
{
	float gl_x, gl_y;
	windowToOpenGL(x, y, width, height, gl_x, gl_y);
	if (left_button == true)
	{
	}
	glutPostRedisplay();
}
GLvoid SpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		player.keyDown(GLUT_KEY_UP);
		break;
	case GLUT_KEY_DOWN:
		player.keyDown(GLUT_KEY_DOWN);
		break;
	case GLUT_KEY_LEFT:
		player.keyDown(GLUT_KEY_LEFT);
		break;
	case GLUT_KEY_RIGHT:
		player.keyDown(GLUT_KEY_RIGHT);
		break;
	}
	glutPostRedisplay();  // ȭ�� ����
}
GLvoid SpecialKeysUp(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		player.keyUp(GLUT_KEY_UP);
		break;
	case GLUT_KEY_DOWN:
		player.keyUp(GLUT_KEY_DOWN);
		break;
	case GLUT_KEY_LEFT:
		player.keyUp(GLUT_KEY_LEFT);
		break;
	case GLUT_KEY_RIGHT:
		player.keyUp(GLUT_KEY_RIGHT);
		break;
	}
	glutPostRedisplay();  // ȭ�� ����
}