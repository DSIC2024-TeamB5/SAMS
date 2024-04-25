#include "SIMModel.h"
<<<<<<< HEAD
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
>>>>>>> dev
