#pragma once
#include "include.h"

extern GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
extern GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
extern GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü
extern GLint width, height;

//����

// ���ε� ����
extern std::vector<GLfloat> vertexArray[3];
extern std::vector<GLfloat> colorArray[3];
extern std::vector<GLfloat> normalArray[3];
extern std::vector<GLfloat> textureArray[3];

// ���ε�
extern GLuint vao_player, vbo_player[4], vao_ball, vbo_ball[4], vao_goalpost, vbo_goalpost[4];

// ���ؽ� ����
extern int PlayerVertexCount, BallVertexCount, GoalPostVertexCount;

// ---------------------------------------------------------------------------------------------------------
// �Լ���
void windowToOpenGL(int window_x, int window_y, int window_width, int window_height, float& gl_x, float& gl_y);
char* filetobuf(const char* file);
void deleteTexture(GLuint textureID);
bool checkSegmentCollision(glm::vec3 start, glm::vec3 end, glm::vec3 goalPos, glm::vec3 goalScale);

//timer
void TimerFunction(int value);

// shader
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();

// obj ������ �Ľ�
struct VertexNormal {
	float x, y, z;     // Vertex coordinates
	float nx, ny, nz;  // Normal coordinates

};
struct Vertex {
	float x, y, z;
};
struct TextureCoord {
	float u, v;
};
struct Normal {
	float nx, ny, nz;
};
struct Face {
	std::vector<std::tuple<int, int, int>> vertices; // (vertex_index, tex_coord_index, normal_index)
};
struct ObjData {
	std::vector<Vertex> vertices;
	std::vector<TextureCoord> texCoords;
	std::vector<Normal> normals;
	std::vector<Face> faces;
};

ObjData parseObj(const std::string& filePath);
void convertToGLArrays(const ObjData& objData, std::vector<GLfloat>& vertexArray, std::vector<GLfloat>& colorArray, std::vector<GLfloat>& normalArray, std::vector<GLfloat>& texCoordArray);
void loadObj();

// bind array
void InitBuffer();

// BMP ���� �ε�
GLuint loadBMP(const char* filepath);

// ��ȯ
void viewTransform();
void projectionTransform();
void make_Light();

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���� �����(������)
void MakeSphere(GLfloat arr[][3], GLfloat normal[][3], GLfloat cx, GLfloat cy, GLfloat cz, GLfloat radius, int first_index);
void MakeShape(GLfloat Shape[][3], GLfloat normal[][3], GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, int first_index, std::string shape);
void MakeColor(GLfloat arr[][3], int first_index, int index_count, GLfloat color[3]);
