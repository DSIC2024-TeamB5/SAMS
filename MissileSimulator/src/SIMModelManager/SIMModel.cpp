#include "SIMModel.h"

#include <math.h>

#include <iostream>

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
  int mslX = mData->x;
  int mslY = mData->y;

  float deltaX, deltaY;
  float theta;

  deltaX = abs(targetX - mslX);
  deltaY = abs(targetY - mslY);

  // hit process
  if (deltaX <= 30.0 && deltaY <= 30.0) {
    mData->MissileBomb = 2;
    return;
  }

  theta = abs(atan(deltaY / deltaX));

  if (mslX < targetX)
    mslX = mslX + (mslSpeed * cos(theta) / 100);
  else
    mslX = mslX - (mslSpeed * cos(theta) / 100);

  if (mslY < targetY)
    mslY = mslY + (mslSpeed * sin(theta) / 100);
  else
    mslY = mslY - (mslSpeed * sin(theta) / 100);

  // update real data position
  mData->x = mslX;
  mData->y = mslY;
}

// void SIMModel::start() {
//   currentX = initX;
//   currentY = initY;
//
//   StartFlag = true;
//   MissileBomb = 1;
//
//   setMissilePosition(currentX, currentY,
//                      1700);
//
//   isThreadRunning = true;
//   simThread =
//       new std::thread(std::bind(&SIMModel::threatSimulationThread, this));
// }

// void SIMModel::stop() {
//   isThreadRunning = false;
//
//   if (simThread) {
//     simThread->join();
//     delete simThread;
//     simThread = nullptr;
//   }
// }

// void SIMModel::threatSimulationThread() {
//   while (isThreadRunning) {
//     RecvThreatPosition(targetX, targetY, targetspeed, 0);
//     updateMissilePosition();
//
//     // the code below may be done by updating the target destination
//     // currentX, currentY
//
//     // check the range and end condition
//     if ((abs(currentX - targetX) <= 30) && (abs(currentY - targetY) <= 30)) {
//       MissileBomb = 2;  // success bomb
//       std::cout << "¸¶Áö¸· DATA : Threat position : ( ¹Ì»çÀÏ X : " <<
//       currentX
//                 << ", ¹Ì»çÀÏ Y : " << currentY << ", Å¸°Ù X : " << targetX
//                 << ", Å¸°Ù  Y : " << targetY << ", ÆøÆÈÇÔ -> " << MissileBomb
//                 << ")" << std::endl;  // for test
//       isThreadRunning = false;
//       break;
//     }
//
//     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//   }
// }