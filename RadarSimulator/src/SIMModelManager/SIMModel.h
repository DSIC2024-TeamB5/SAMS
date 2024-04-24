#pragma once
#include <chrono>
#include <cmath>
#include <functional>
#include <thread>

struct SharedData {
  float radarX, radarY;
  float enemyDistance;
  bool isTargetDetect;
  SharedData()
      : radarX(0.0), radarY(0.0), enemyDistance(0.0), isTargetDetect(false) {}
};

class SIMModel {
 public:
  SIMModel(SharedData*);
  ~SIMModel();

  void setPosition(float x, float y);             //[km]
  void calculateEnemyDistance(float x, float y);  //[km]
  void detectEnemy();

 private:
  SharedData* mData;
  const int detectRange = 200;  //[km]
};
