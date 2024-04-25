#pragma once
#include <chrono>
#include <cmath>
#include <functional>
#include <thread>

/**
 * @brief 공유 자원 구조체
 * @details 파사드 패턴을 이용하여 클래스 간 의존도를 낮추기 위해 만들어진 공유자원 구조체이다.
 */
struct SharedData {
  float radarX; /**< 레이다 모의기의 x 좌표*/
  float radarY; /**< 레이다 모의기의 y 좌표*/
  float enemyDistance;  /**< 레이다와 공중위협 사이 거리, km단위*/
  bool isTargetDetect; /**< 레이다가 공중위협을 탐지했는지 여부*/
  
  /**
   * @brief 새로운 Shared Data 객체를 생성
   * @brief 객체의 맴버변수 초기화를 진행한다
   */
  SharedData()
      : radarX(0.0), radarY(0.0), enemyDistance(0.0), isTargetDetect(false) {}
};

/**
 * @brief 레이다 모의 클래스
 * @details 레이다 모의 수행을 직접 진행하는 클래스이다.
 */
class SIMModel {
 public:
  SIMModel(SharedData*);
  ~SIMModel();

  void setPosition(float x, float y);             //[km]
  void calculateEnemyDistance(float x, float y);  //[km]
  void detectEnemy();

 private:
  SharedData* mData;
  const int detectRange = 200;  //[km]
};
