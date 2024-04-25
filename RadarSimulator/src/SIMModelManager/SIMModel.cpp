#include "SIMModel.h"

#include <math.h>

#include <iostream>

using namespace std;

/**
 * @brief 새로운 SIMModel::SIMModel 객체를 생성한다.
 * 
 * @param data : 공유 자원 포인터
 */
SIMModel::SIMModel(SharedData* data) : mData(data) {}


SIMModel::~SIMModel() {
  // if need be, write your code
}

/**
 * @brief 레이다 위치 설정
 * @details x, y 좌표를 입력 받아 레이다의 초기 위치를 설정한다.
 * @param x 레이다 x 좌표
 * @param y 레이다 y 좌표
 */
void SIMModel::setPosition(float x, float y) {
  mData->radarX = x;
  mData->radarY = y;
}

/**
 * @brief 공중 위협 거리 계산
 * @details 공중 위협의 좌표를 입력받아 레이다와 공중위협 사이의 거리를 계산하여 업데이트한다.
 * @param x 공중 위협 x 좌표
 * @param y 공중 위협 y 좌표
 */
void SIMModel::calculateEnemyDistance(float x, float y) {
  double deltaX, deltaY;
  deltaX = (mData->radarX - x);
  deltaY = (mData->radarY - y);

  mData->enemyDistance = (float)sqrt(deltaX * deltaX + deltaY * deltaY);
}

/**
 * @brief 공중 위협을 탐지한다.
 * @details 레이다와 공중 위협 사이의 거리가 레이다 탐지 거리보다 작다면 SharedData의 isTargetDetect 를 true 로 설정한다. \n
 * @details 그렇지 않다면 SharedData 의 isTargetDetect 를 false 로 설정한다.
 */
void SIMModel::detectEnemy() {
  if (mData->enemyDistance <= detectRange)
    mData->isTargetDetect = true;
  else
    mData->isTargetDetect = false;
}