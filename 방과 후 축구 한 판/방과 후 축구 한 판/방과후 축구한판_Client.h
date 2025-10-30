#pragma once
#include "Ball.h"
#include "Keeper.h"
#include "Camera.h"
#include "Light.h"

// ∑ª¥ı∏µ
GLvoid drawScene();
GLvoid Reshape(int w, int h);

// ¡∂¿€
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void SpecialKeys(int key, int x, int y);
void SpecialKeysUp(int key, int x, int y);
