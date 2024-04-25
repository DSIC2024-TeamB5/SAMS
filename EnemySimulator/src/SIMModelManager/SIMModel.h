#pragma once
<<<<<<< HEAD
#include <thread>
#include <chrono>
#include <functional>

class SIMModel
{
public:
	SIMModel();
	~SIMModel();

	void setThreatPosition(double x, double y);
	void setThreatTargetPosition(double x, double y);
	void start();
	void stop();

private:
	std::thread* simThread = nullptr;

	double initX, initY;
	double targetX, targetY;
	double speed;
	double currentX, currentY;

	bool isThreadRunning = false;

private:
	void threatSimulationThread();
	void updateThreatPosition();
};

=======
#include <chrono>
#include <functional>
#include <thread>

struct Coords  // X, Y, Z 좌표값을 갖는 struct
{
  float x;
  float y;
  Coords() : x(0.0), y(0.0) {}
};

struct SharedData {
  Coords enemyPosition;
};

class EnemySimulation  // 공중위협의 시작/도착위치를 설정하는 class
{
 private:
  SharedData* mData;
  Coords destination;
  Coords dirVec;
  Coords enemyInitPos;

  int enemyType;
  int enemyStatus;
  float enemySpeed;
  float Range;
  float CummulRange;
  // bool isThreadRunning = false;
  std::thread* simThread = nullptr;

 public:
  bool isThreadRunning = false;

  EnemySimulation(SharedData* data);
  ~EnemySimulation();

  void SetDestPosition(float x, float y);  // 공중위협 초기위치 설정 함수

  void SetInitPosition(float x, float y);  // 공중위협 목적지 설정 함수

  void SetCurrentPosition(float x, float y);  // 공중위협 현재위치 설정 함수

  void SetEnemySpeed(float v);  // 공중위협 속도 설정

  void SetEnemyType(int type);  // 공중위협 타입 설정

  void EnemyDown();  // 공중위협이 격추되면 status를 1로 변경

  float SubsTwo(float a, float b);  // 두 입력값의 차이를 반환하는 함수

  float CalcNorm(float x, float y);  // 입력한 벡터의 norm을 계산하는 함수

  float CalcRange(float x0, float x1, float y0,
                  float y1);  // 두 지점 사이의 거리 계산 함수

  void SetVec();  // 방향벡터 계산 함수

  void MoveEnemy();  // 공중위협의 현재 위치를 변경하는 함수
};
>>>>>>> dev
