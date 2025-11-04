#pragma once
#include "include.h"
// ---------------------------------------------------------------------------------------------------------
// ÇÔ¼öµé
void windowToOpenGL(int window_x, int window_y, int window_width, int window_height, float& gl_x, float& gl_y);
char* filetobuf(const char* file);
bool checkSegmentCollision(glm::vec3 start, glm::vec3 end, glm::vec3 goalPos, glm::vec3 goalScale);

//timer
void TimerFunction(int value);

