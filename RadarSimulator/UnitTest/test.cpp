#include "pch.h"
#include <math.h>

TEST(TestCaseName, TestName) {
	SharedData data;
	SIMModel model(&data);


	// setPosition 테스트
	model.setPosition(0, 0);
	EXPECT_EQ(0, (int)data.radarX);
	EXPECT_EQ(0, (int)data.radarY);


	// calulateEnemyDistance 테스트1
	float targetX0, targetY0;
	double delX0, delY0;
	targetX0 = 200 * pow(10, 3);
	targetY0 = 200 * pow(10, 3);
	delX0 = (targetX0 - 0);
	delY0 = (targetY0 - 0);

	model.calculateEnemyDistance(targetX0, targetY0);
	EXPECT_EQ((float)sqrt(delX0 * delX0 + delY0 * delY0), data.enemyDistance);


	// detectEnemy 테스트1(false)
	model.detectEnemy();
	EXPECT_EQ(0, data.isTargetDetect);


	// calulateEnemyDistance 테스트2
	float targetX1, targetY1;
	double delX1, delY1;
	targetX1 = 100 * pow(10, 3);
	targetY1 = 100 * pow(10, 3);
	delX1 = (targetX1 - 0);
	delY1 = (targetY1 - 0);

	model.calculateEnemyDistance(targetX1, targetY1);
	EXPECT_EQ((float)sqrt(delX1 * delX1 + delY1 * delY1), data.enemyDistance);


	// detectEnemy 테스트2(True)
	model.detectEnemy();
	EXPECT_EQ(1, data.isTargetDetect);
	std::cout << "---%%---" << std::endl;
}