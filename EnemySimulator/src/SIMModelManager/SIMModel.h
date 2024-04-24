#pragma once
#include <chrono>
#include <functional>
#include <thread>

struct Coords  // X, Y, Z ��ǥ���� ���� struct
{
  float x;
  float y;
  Coords() : x(0.0), y(0.0) {}
};

struct SharedData {
  Coords enemyPosition;
};

class EnemySimulation  // ���������� ����/������ġ�� �����ϴ� class
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

  void SetDestPosition(float x, float y);  // �������� �ʱ���ġ ���� �Լ�

  void SetInitPosition(float x, float y);  // �������� ������ ���� �Լ�

  void SetCurrentPosition(float x, float y);  // �������� ������ġ ���� �Լ�

  void SetEnemySpeed(float v);  // �������� �ӵ� ����

  void SetEnemyType(int type);  // �������� Ÿ�� ����

  void EnemyDown();  // ���������� ���ߵǸ� status�� 1�� ����

  float SubsTwo(float a, float b);  // �� �Է°��� ���̸� ��ȯ�ϴ� �Լ�

  float CalcNorm(float x, float y);  // �Է��� ������ norm�� ����ϴ� �Լ�

  float CalcRange(float x0, float x1, float y0,
                  float y1);  // �� ���� ������ �Ÿ� ��� �Լ�

  void SetVec();  // ���⺤�� ��� �Լ�

  void MoveEnemy();  // ���������� ���� ��ġ�� �����ϴ� �Լ�
};
