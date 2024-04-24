#pragma once

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
