#include "SIMModel.h"

#include <math.h>

#include <iostream>

constexpr float kSimulateHz = 10.0;
constexpr float kHitRange = 300.0;

SIMModel::SIMModel(SharedData* data) : mData(data) {
  targetX = 0;
  targetY = 0;
  mslSpeed = kMissileSpeed; 
  targetAngle = 0;
}

SIMModel::~SIMModel() {}

void SIMModel::setMissileInfo(float x, float y) {
  mData->x = x;
  mData->y = y;
}

void SIMModel::updateTargetInfo(float x, float y) {
  targetX = x;
  targetY = y;
}

void SIMModel::updateMissilePosition() {
  float mslX = mData->x;
  float mslY = mData->y;

  float deltaX, deltaY;
  float theta;

  deltaX = abs(targetX - mslX);
  deltaY = abs(targetY - mslY);

  // hit process
  if (deltaX <= kHitRange && deltaY <= kHitRange) {
    mData->x = targetX;
    mData->y = targetY;
    mData->MissileBomb = 2;
    return;
  }

  theta = abs(atan(deltaY / deltaX));

  if (mslX < targetX)
    mslX = mslX + (mslSpeed * cos(theta) / kSimulateHz);
  else
    mslX = mslX - (mslSpeed * cos(theta) / kSimulateHz);

  if (mslY < targetY)
    mslY = mslY + (mslSpeed * sin(theta) / kSimulateHz);
  else
    mslY = mslY - (mslSpeed * sin(theta) / kSimulateHz);

  // update real data position
  mData->x = mslX;
  mData->y = mslY;
}