#include "SIMModelManager.h"

void SIMModelManager::scnDeploy(shared_ptr<NOM> nomMsg) {
  float mslX = nomMsg->getValue(_T("MissileInitX"))->toFloat();
  float mslY = nomMsg->getValue(_T("MissileInitY"))->toFloat();
  float targetX = nomMsg->getValue(_T("EnemyInitX"))->toFloat();
  float targetY = nomMsg->getValue(_T("EnemyInitY"))->toFloat();

  mslX *= 1000;
  mslY *= 1000;
  targetX *= 1000;
  targetY *= 1000;

  missile->setMissileInfo(mslX, mslY);
  missile->updateTargetInfo(targetX, targetY);
  notifyStatus();
}

void SIMModelManager::simControl(shared_ptr<NOM> nomMsg) {
  // 1:모의시작, 2:모의현황요청, 3:모의중지, other: N/A
  unsigned int operationtype = nomMsg->getValue(_T("OperationType"))->toUInt();
  if (operationtype == 1) {

    notifyStatus();

  } else if (operationtype == 2) {

    missile->updateMissilePosition();
    notifyMissileInfo();

  } else if (operationtype == 3) {

    notifyStatus();

  }
}

void SIMModelManager::mslLaunch(shared_ptr<NOM> nomMsg) {
  notifyStatus();
}

void SIMModelManager::atsPosition(shared_ptr<NOM> nomMsg) {
  float x = nomMsg->getValue(_T("EnemyCurX"))->toFloat();
  float y = nomMsg->getValue(_T("EnemyCurY"))->toFloat();
  x *= 1000;
  y *= 1000;
  missile->updateTargetInfo(x, y);
  notifyStatus();
}

void SIMModelManager::notifyStatus() {
  auto _nomMsg = meb->getNOMInstance(name, _T("SIM_STATUS"));
  _nomMsg->setValue(_T("MessageId"), &NUInteger(1003));
  _nomMsg->setValue(_T("MessageSize"), &NUInteger(12));
  _nomMsg->setValue(_T("SimulatorState"), &NUInteger(2));
  this->sendMsg(_nomMsg);
}

void SIMModelManager::notifyMissileInfo() {
  float x = mData->x;
  float y = mData->y;
  x /= 1000;
  y /= 1000;

  int isShotDown = mData->MissileBomb;
  auto _nomMsg = meb->getNOMInstance(name, _T("MSL_POSITION"));
  _nomMsg->setValue(_T("MessageId"), &NUInteger(1006));
  _nomMsg->setValue(_T("MessageSize"), &NUInteger(20));
  _nomMsg->setValue(_T("MissileCurX"), &NFloat(x));
  _nomMsg->setValue(_T("MissileCurY"), &NFloat(y));
  _nomMsg->setValue(_T("IsShotDown"), &NUInteger(isShotDown));
  this->sendMsg(_nomMsg);
}

bool SIMModelManager::start() {
  // if need be, write your code
  mData = new SharedData;
  missile = new SIMModel(mData);
  // binding message

  function<void(shared_ptr<NOM>)> _scn_deploy;
  function<void(shared_ptr<NOM>)> _sim_control;
  function<void(shared_ptr<NOM>)> _ats_position;
  function<void(shared_ptr<NOM>)> _msl_launch;

  _scn_deploy = bind(&SIMModelManager::scnDeploy, this, placeholders::_1);
  _sim_control = bind(&SIMModelManager::simControl, this, placeholders::_1);
  _ats_position = bind(&SIMModelManager::atsPosition, this, placeholders::_1);
  _msl_launch = bind(&SIMModelManager::mslLaunch, this, placeholders::_1);
  

  msgMethodMap.insert(make_pair(_T("SCN_DEPLOY"), _scn_deploy));
  msgMethodMap.insert(make_pair(_T("SIM_CONTROL"), _sim_control));
  msgMethodMap.insert(make_pair(_T("ATS_POSITION"), _ats_position));
  msgMethodMap.insert(make_pair(_T("MSL_LAUNCH"), _msl_launch));

  return true;
}

bool SIMModelManager::stop() {
  // if need be, write your code
  delete mData;
  delete missile;
  return true;
}