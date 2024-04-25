#pragma once
<<<<<<< HEAD
#include <thread>
#include <chrono>
#include <functional>

class SIMModel
{
public:
	SIMModel();
	~SIMModel();

	void setThreatPosition(double x, double y);
	void setThreatTargetPosition(double x, double y);
	void start();
	void stop();

private:
	std::thread* simThread = nullptr;

	double initX, initY;
	double targetX, targetY;
	double speed;
	double currentX, currentY;

	bool isThreadRunning = false;

private:
	void threatSimulationThread();
	void updateThreatPosition();
};

=======

struct SharedData {
  float x;
  float y;
  int mslCount;
  SharedData() : x(0.0), y(0.0), mslCount(4) {}
};

class SIMModel {
public:
  SIMModel(SharedData* data);
  ~SIMModel();
  void setPosition(float x, float y);
  void launchMissile();

private:
  SharedData* mData;
};
>>>>>>> dev
