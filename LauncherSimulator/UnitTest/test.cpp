#include "pch.h"

TEST(LaunchTest, Launcher) {
  SharedData *data = new SharedData;
  SIMModel *launcher = new SIMModel(data);

  launcher->launchMissile();

  EXPECT_EQ(3, data->mslCount);

  delete launcher;
  delete data;
}

TEST(PositionSetTest, Launcher) {
  SharedData *data = new SharedData;
  SIMModel *launcher = new SIMModel(data);

  float x = 32.1, y = 29.97;
  launcher->setPosition(x, y);

  EXPECT_EQ(x, data->x);
  EXPECT_EQ(y, data->y);

  delete launcher;
  delete data;
}