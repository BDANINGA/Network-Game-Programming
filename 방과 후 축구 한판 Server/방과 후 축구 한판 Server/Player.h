#pragma once
#include "Ball.h"

class Player : Object
{
public:
	Player();
	Player(GLfloat x, GLfloat y, GLfloat z);

	~Player();

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	float getBallDistance(Ball& ball);

	void setPosition(GLfloat x, GLfloat y, GLfloat z);

	void Move(Ball& ball, bool keeper_has_ball);

	void Sprint();
	void Walk();
	bool isSprint();

	void DoTackle();
	bool isTackle();

	void Shoot(Ball& ball);

	bool isShooting();
	void changeShooting(bool shootinginprogress);
	bool isCurve();
	void changeCurve(bool curve);
	bool isStrong();
	void changeStrong(bool strong);

	bool ishasBall();
	void toggleHasBall(bool has_ball);

	void keyDown(int keys);
	void keyUp(int keys);
	bool isKey(int keys);



private:
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);  // 속도
	glm::vec3 direction{};  // 이동 방향
	float acceleration = 0.05f;  // 플레이어의 가속도

	float max_speed = 10.0f; // 최대 속도
	const float deceleration = 0.01f; // 감속 (가속도와 반대)
	
	float distance{};	// 공과 플레이어 사이의 거리
	float shootingpower{};  // 슈팅 파워 변수 (0.0 ~ MAX_SHOOTING_POWER 범위)
	const float max_shootingpower = 50.0f;  // 최대 슈팅 파워
	const float shooting_increase = 1.0f;  // 슈팅 파워 증가량

	time_t tacklecool{};	// Tackle의 쿨타임 Session Loop에서 연산.

	bool sprint{};
	bool has_ball= true;	// 첫 렌더링을 위해 true로 바꿈
	bool keystates[256]{};	// 키보드 상태
	bool shootingInprogress{};  // 슈팅 진행 중 여부 (d 키가 눌린 상태인지)
	bool curve = false;			// 감아차기
	bool strong = false;		// 파워슛
	bool tackle = false;		// 태클

};

// TackleEvent - 태클로 공 소유권 이전 여부를 서버에서 관리해야한다.
void TackleEvent(Player* player, int count, Ball& ball);