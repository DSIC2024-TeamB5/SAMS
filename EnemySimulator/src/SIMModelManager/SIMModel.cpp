/*
 * @file SIMModel.cpp
 * @breif wow~
 * @author Me
 * @data 24/04/24
 */

#include "SIMModel.h"

#include <cmath>
#include <iostream>

EnemySimulation::EnemySimulation(SharedData* data) : mData(data) {
  enemyType = 0;
  enemyStatus = 0;
  enemySpeed = 0.0;
  Range = 0.0;
  CummulRange = 0.0;
}

EnemySimulation::~EnemySimulation() {}

void EnemySimulation::SetDestPosition(float x, float y) {
  destination.x = x;
  destination.y = y;
}

void EnemySimulation::SetInitPosition(float x, float y) {
  enemyInitPos.x = x;
  enemyInitPos.y = y;
}

void EnemySimulation::SetCurrentPosition(float x, float y) {
  mData->enemyPosition.x = x;
  mData->enemyPosition.y = y;
}

void EnemySimulation::SetEnemySpeed(float v) { enemySpeed = v * 340; }

void EnemySimulation::SetEnemyType(int type) { enemyType = type; }

void EnemySimulation::EnemyDown() { enemyStatus = 1; }

float EnemySimulation::SubsTwo(float a, float b) { return b - a; }

float EnemySimulation::CalcNorm(float x, float y) {
  return sqrt((x * x) + (y * y));
}

float EnemySimulation::CalcRange(float x0, float x1, float y0, float y1) {
  return CalcNorm(SubsTwo(x0, x1), SubsTwo(y0, y1));
}

void EnemySimulation::SetVec() {
  dirVec.x = SubsTwo(enemyInitPos.x, destination.x);
  dirVec.y = SubsTwo(enemyInitPos.y, destination.y);

  float R = CalcNorm(dirVec.x, dirVec.y);
  dirVec.x /= R;
  dirVec.y /= R;
}

void EnemySimulation::MoveEnemy() {
  float tempX = (mData->enemyPosition.x) + (enemySpeed * dirVec.x * 0.1);
  float tempY = (mData->enemyPosition.y) + (enemySpeed * dirVec.y * 0.1);
  CummulRange +=
      CalcRange(mData->enemyPosition.x, tempX, mData->enemyPosition.y, tempY);
  SetCurrentPosition(tempX, tempY);
}