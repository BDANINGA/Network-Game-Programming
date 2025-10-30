#define _CRT_SECURE_NO_WARNINGS
#include "function.h"

#define M_PI 3.14159265358979323846

// �Լ���
void windowToOpenGL(int window_x, int window_y, int window_width, int window_height, float& gl_x, float& gl_y) {
    // OpenGL ��ǥ���� x�� ��ȯ: ������ ��ǥ�� -1.0 ~ 1.0 ���̷� ����ȭ
    gl_x = (2.0f * window_x) / window_width - 1.0f;

    // OpenGL ��ǥ���� y�� ��ȯ: ������ ��ǥ�� ���Ʒ��� �ݴ��̹Ƿ� y�� ��ȯ
    gl_y = 1.0f - (2.0f * window_y) / window_height;
}
char* filetobuf(const char* file)
{
    FILE* fptr;
    long length;
    char* buf;
    fptr = fopen(file, "rb"); // Open file for reading
    if (!fptr) // Return NULL on failure
        return NULL;
    fseek(fptr, 0, SEEK_END); // Seek to the end of the file
    length = ftell(fptr); // Find out how many bytes into the file we are
    buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
    fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
    fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
    fclose(fptr); // Close the file
    buf[length] = 0; // Null terminator
    return buf; // Return the buffer 
}
bool checkSegmentCollision(glm::vec3 start, glm::vec3 end, glm::vec3 goalPos, glm::vec3 goalScale) {
    glm::vec3 boxMin = goalPos - goalScale;
    glm::vec3 boxMax = goalPos + goalScale;

    for (int i = 0; i < 3; ++i) {
        float boxMinCoord = boxMin[i];
        float boxMaxCoord = boxMax[i];

        if (start[i] < boxMinCoord && end[i] < boxMinCoord || start[i] > boxMaxCoord && end[i] > boxMaxCoord)
            return false;

        if (start[i] < boxMinCoord || end[i] > boxMaxCoord) {
            float t = (boxMinCoord - start[i]) / (end[i] - start[i]);
            if (t > 0.0f && t < 1.0f) {
                return true;
            }
        }
    }

    return true;
}

// tiemer
void TimerFunction(int value)
{
    glutTimerFunc(25, TimerFunction, 1);
    glutPostRedisplay();
}