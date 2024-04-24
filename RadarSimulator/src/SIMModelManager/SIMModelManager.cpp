#include "SIMModelManager.h"

void SIMModelManager::scnDeploy(shared_ptr<NOM> nomMsg) {
  float radarinitX = nomMsg->getValue(_T("RadarInitX"))->toFloat();
  float radarinitY = nomMsg->getValue(_T("RadarInitY"))->toFloat();
  mRadar->setPosition(radarinitX, radarinitY);
  notifyStatus();
}

void SIMModelManager::simControl(shared_ptr<NOM> nomMsg) {
  // 1:모의시작, 2:모의현황요청, 3:모의중지, other: N/A
  unsigned int operationtype = nomMsg->getValue(_T("OperationType"))->toUInt();

  if (operationtype == 1) {
    notifyStatus();
  } else if (operationtype == 2) {
    mRadar->detectEnemy();
    bool istargetdetect = mData->isTargetDetect;

    auto _nomMsg = meb->getNOMInstance(name, _T("ROS_DETECTION"));
    _nomMsg->setValue(_T("MessageId"), &NUInteger(1004));
    _nomMsg->setValue(_T("MessageSize"), &NUInteger(12));
    _nomMsg->setValue(_T("IsDetected"), &NUInteger((int)istargetdetect));

    this->sendMsg(_nomMsg);  // 현재 탐지 여부 전~송
  } else if (operationtype == 3) {
    notifyStatus();
  }
}

void SIMModelManager::atsPosition(shared_ptr<NOM> nomMsg) {
  float enemycurX = nomMsg->getValue(_T("EnemyCurX"))->toFloat();
  float enemycurY = nomMsg->getValue(_T("EnemyCurY"))->toFloat();
  mRadar->calculateEnemyDistance(enemycurX, enemycurY);
}

void SIMModelManager::notifyStatus() {
  auto _nomMsg = meb->getNOMInstance(name, _T("SIM_STATUS"));
  _nomMsg->setValue(_T("MessageId"), &NUInteger(1003));
  _nomMsg->setValue(_T("MessageSize"), &NUInteger(12));
  _nomMsg->setValue(_T("SimulatorState"), &NUInteger(3));
  this->sendMsg(_nomMsg);
}

bool SIMModelManager::start() {
  // if need be, write your code
  mData = new SharedData;
  mRadar = new SIMModel(mData);

  // binding message
  function<void(shared_ptr<NOM>)> _scn_deploy;
  function<void(shared_ptr<NOM>)> _sim_control;
  function<void(shared_ptr<NOM>)> _ats_position;

  _scn_deploy = bind(&SIMModelManager::scnDeploy, this, placeholders::_1);
  _sim_control = bind(&SIMModelManager::simControl, this, placeholders::_1);
  _ats_position = bind(&SIMModelManager::atsPosition, this, placeholders::_1);

  msgMethodMap.insert(make_pair(_T("SCN_DEPLOY"), _scn_deploy));
  msgMethodMap.insert(make_pair(_T("SIM_CONTROL"), _sim_control));
  msgMethodMap.insert(make_pair(_T("ATS_POSITION"), _ats_position));

  return true;
}

bool SIMModelManager::stop() {
  // if need be, write your code
  delete mData;
  delete mRadar;
  return true;
}