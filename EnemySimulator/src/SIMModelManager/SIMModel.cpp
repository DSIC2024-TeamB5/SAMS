<<<<<<< HEAD
#include "SIMModel.h"
#include <math.h>
#include <iostream>

SIMModel::SIMModel()
{
	initX = 0;
	initY = 0;
	targetX = 0;
	targetY = 0;
	speed = 0;
}

SIMModel::~SIMModel()
{
	// if need be, write your code
}

void SIMModel::setThreatPosition(double x, double y)
{
	initX = x;
	initY = y;
}

void SIMModel::setThreatTargetPosition(double x, double y)
{
	targetX = x;
	targetY = y;

	// simple model
	speed = sqrt((targetX - initX) * (targetX - initX) + (targetY - initY)) / 30.0;
}

void SIMModel::start()
{
	currentX = initX;
	currentY = initY;

	isThreadRunning = true;
	simThread = new std::thread(std::bind(&SIMModel::threatSimulationThread, this));
}

void SIMModel::stop()
{
	isThreadRunning = false;

	if (simThread)
	{
		simThread->join();
		delete simThread;
		simThread = nullptr;
	}
}

void SIMModel::threatSimulationThread()
{
	while (isThreadRunning)
	{
		updateThreatPosition();

		// the code below may be done by updating the target destination
		// currentX, currentY

		// check the range and end condition
		if ((initX < targetX && currentX > targetX) || (initX > targetX&& currentX < targetX))
		{
			isThreadRunning = false;
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void SIMModel::updateThreatPosition()
{
	// you can change the model

	double deltaX, deltaY;
	double theta;

	deltaX = targetX - initX;
	deltaY = targetY - initY;

	if (deltaX == 0.0 || deltaY == 0.0)
		return;

	theta = abs(atan(deltaY / deltaX));

	if (initX < targetX)
		currentX = currentX + speed * cos(theta);
	else
		currentX = currentX - speed * cos(theta);

	if (initY < targetY)
		currentY = currentY + speed * sin(theta);
	else
		currentY = currentY - speed * sin(theta);

	std::cout << "Threat position : (" << currentX << ", " << currentY << ")" << std::endl;
}
=======
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
>>>>>>> dev
