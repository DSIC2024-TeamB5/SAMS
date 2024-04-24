#include "SIMModel.h"
#include <iostream>

SIMModel::SIMModel(SharedData* data) : mData(data) {}

SIMModel::~SIMModel() { /*delete mData;*/ }

void SIMModel::setPosition(float x, float y) {
  mData->x = x;
  mData->y = y;
}

void SIMModel::launchMissile() {
  if (mData->mslCount <= 0) throw("missile count under 0");

  mData->mslCount--;
}