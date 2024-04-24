#include "SIMModelManager.h"

void SIMModelManager::scnDeploy(shared_ptr<NOM> nomMsg) {
  float initX = nomMsg->getValue(_T("EnemyInitX"))->toFloat() * 1000;
  float initY = nomMsg->getValue(_T("EnemyInitY"))->toFloat() * 1000;
  float destX = nomMsg->getValue(_T("EnemyDestX"))->toFloat() * 1000;
  float destY = nomMsg->getValue(_T("EnemyDestY"))->toFloat() * 1000;
  float speed = nomMsg->getValue(_T("EnemySpeed"))->toFloat();
  int type = nomMsg->getValue(_T("EnemyType"))->toInt();

  enemySim->SetInitPosition(initX, initY);
  enemySim->SetCurrentPosition(initX, initY);
  enemySim->SetDestPosition(destX, destY);
  enemySim->SetEnemySpeed(speed);
  enemySim->SetEnemyType(type);
  enemySim->SetVec();

  notifyStatus();
}

void SIMModelManager::simControl(shared_ptr<NOM> nomMsg) {
  auto _id = nomMsg->getValue(_T("OperationType"))->toInt();
  if (_id == 1) {
    notifyStatus();
  } else if (_id == 2) {
    enemySim->MoveEnemy();
    notifyPosition();
  } else if (_id == 3) {
    notifyStatus();
  }
}

void SIMModelManager::atsShotDown(shared_ptr<NOM> nomMsg) {
  enemySim->EnemyDown();
}

void SIMModelManager::notifyPosition() {
  auto nomMsg = meb->getNOMInstance(name, _T("ATS_POSITION"));
  nomMsg->setValue(_T("MessageId"), &NUInteger(1008));
  nomMsg->setValue(_T("MessageSize"), &NUInteger(16));
  nomMsg->setValue(_T("EnemyCurX"), &NFloat(mData->enemyPosition.x / 1000));
  nomMsg->setValue(_T("EnemyCurY"), &NFloat(mData->enemyPosition.y / 1000));
  this->sendMsg(nomMsg);
}

void SIMModelManager::notifyStatus() {
  auto nomMsg = meb->getNOMInstance(name, _T("SIM_STATUS"));
  nomMsg->setValue(_T("MessageId"), &NUInteger(1003));
  nomMsg->setValue(_T("MessageSize"), &NUInteger(12));
  nomMsg->setValue(_T("SimulatorState"), &NUInteger(1));
  this->sendMsg(nomMsg);
}

bool SIMModelManager::start() {
  mData = new SharedData;
  enemySim = new EnemySimulation(mData);

  // binding message
  function<void(shared_ptr<NOM>)> SCN_DEPLOY;
  function<void(shared_ptr<NOM>)> SIM_CONTROL;
  function<void(shared_ptr<NOM>)> ATS_SHOT_DOWN;

  SCN_DEPLOY = bind(&SIMModelManager::scnDeploy, this, placeholders::_1);
  SIM_CONTROL = bind(&SIMModelManager::simControl, this, placeholders::_1);
  ATS_SHOT_DOWN = bind(&SIMModelManager::atsShotDown, this, placeholders::_1);

  msgMethodMap.insert(make_pair(_T("SCN_DEPLOY"), SCN_DEPLOY));
  msgMethodMap.insert(make_pair(_T("SIM_CONTROL"), SIM_CONTROL));
  msgMethodMap.insert(make_pair(_T("ATS_SHOT_DOWN"), ATS_SHOT_DOWN));

  return true;
}

bool SIMModelManager::stop() {
  delete mData;
  delete enemySim;
  return true;
}