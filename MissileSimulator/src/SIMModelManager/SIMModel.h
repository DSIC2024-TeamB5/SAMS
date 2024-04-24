#pragma once
#include <chrono>
#include <functional>
#include <thread>

constexpr int kMissileSpeed = 1700; // test : 5mach -> 1700m/s //1mach : 340

struct SharedData {
  float x;
  float y;
  int MissileBomb; // 1 : target alive, 2: missile hit, target destroy
  SharedData() : x(0.0), y(0.0), MissileBomb(1) {}
};

class SIMModel {
 public:
  SIMModel(SharedData*);
  ~SIMModel();
  void setMissileInfo(float x, float y);
  void updateMissilePosition();
  void updateTargetInfo(float x, float y);
  //void start();
  //void stop();

private:
  SharedData* mData;
  float targetX, targetY;
  float mslSpeed, targetSpeed;
  float targetAngle;
  //bool startFlag;

 // std::thread* simThread = nullptr;

 // bool isThreadRunning = false;

 //private:
 // void threatSimulationThread();
 // void updateMissilePosition();
};
