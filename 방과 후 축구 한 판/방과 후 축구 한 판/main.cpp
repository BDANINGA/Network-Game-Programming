//--- ���� �Լ�
#include "����� �౸����_Client.h"

//--- �ʿ��� ���� ����
GLint width, height;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

// ���ε� ����
std::vector<GLfloat> vertexArray[3];
std::vector<GLfloat> colorArray[3];
std::vector<GLfloat> normalArray[3];
std::vector<GLfloat> textureArray[3];

// ���ε�
GLuint vao_player, vbo_player[4], vao_ball, vbo_ball[4], vao_goalpost, vbo_goalpost[4];

// ���ؽ� ����
int PlayerVertexCount{}, BallVertexCount{}, GoalPostVertexCount{};

//����
FMOD::System* ssystem;
FMOD::Sound* s_bgm, * s_goal, * s_touch, * s_shoot;
FMOD::Channel* c_bgm = 0;
FMOD::Channel* c_goal = 0;
FMOD::Channel* c_touch = 0;
FMOD::Channel* c_shoot = 0;
FMOD_RESULT result;
void* extradriverdata = 0;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	srand(time(NULL));
	width = 1200;
	height = 800;
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	//--- ���̴� �о�ͼ� ���̴� ���α׷� �����
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	shaderProgramID = make_shaderProgram(); //--- ���̴� ���α׷� �����

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(SpecialKeys);       // ����Ű �Է� �ݹ� �Լ�
	glutSpecialUpFunc(SpecialKeysUp);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}






