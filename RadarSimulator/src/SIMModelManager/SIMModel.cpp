#include "SIMModel.h"

#include <math.h>

#include <iostream>

using namespace std;

SIMModel::SIMModel(SharedData* data) : mData(data) {}

SIMModel::~SIMModel() {
  // if need be, write your code
}

void SIMModel::setPosition(float x, float y) {
  mData->radarX = x;
  mData->radarY = y;
}

void SIMModel::calculateEnemyDistance(float x, float y) {
  double deltaX, deltaY;
  deltaX = (mData->radarX - x);
  deltaY = (mData->radarY - y);

  mData->enemyDistance = (float)sqrt(deltaX * deltaX + deltaY * deltaY);
}

void SIMModel::detectEnemy() {
  if (mData->enemyDistance <= detectRange)
    mData->isTargetDetect = true;
  else
    mData->isTargetDetect = false;
}