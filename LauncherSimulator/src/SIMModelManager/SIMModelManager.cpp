#include "SIMModelManager.h"

void SIMModelManager::scnDeploy(shared_ptr<NOM> nomMsg) {
  int x = nomMsg->getValue(_T("LauncherInitX"))->toUInt();
  int y = nomMsg->getValue(_T("LauncherInitY"))->toUInt();

  mData->mslCount = 4;
  mLauncher->setPosition(x, y);
  notifyStatus();
}

void SIMModelManager::simControl(shared_ptr<NOM> nomMsg) {
  // do nothing
  // only notification
  notifyStatus();
}

void SIMModelManager::mslLaunch(shared_ptr<NOM> nomMsg) {
  mLauncher->launchMissile();
  notifyStatus();
}

void SIMModelManager::notifyStatus() {
  auto nomMsg = meb->getNOMInstance(name, _T("SIM_STATUS"));
  nomMsg->setValue(_T("MessageId"), &NUInteger(1003));
  nomMsg->setValue(_T("MessageSize"), &NUInteger(12));
  nomMsg->setValue(_T("SimulatorState"), &NUInteger(4));
  sendMsg(nomMsg);
}

bool SIMModelManager::start() {

  mData = new SharedData;
  mLauncher = new SIMModel(mData);

  // binding message

  function<void(shared_ptr<NOM>)> _scn_deploy;
  function<void(shared_ptr<NOM>)> _sim_control;
  function<void(shared_ptr<NOM>)> _msl_launch;

  _scn_deploy = bind(&SIMModelManager::scnDeploy, this, placeholders::_1);
  _sim_control = bind(&SIMModelManager::simControl, this, placeholders::_1);
  _msl_launch = bind(&SIMModelManager::mslLaunch, this, placeholders::_1);

  msgMethodMap.insert(make_pair(_T("SCN_DEPLOY"), _scn_deploy));
  msgMethodMap.insert(make_pair(_T("SIM_CONTROL"), _sim_control));
  msgMethodMap.insert(make_pair(_T("MSL_LAUNCH"), _msl_launch));

  return true;
}

bool SIMModelManager::stop() {
  delete mLauncher;
  delete mData;
  return true;
}