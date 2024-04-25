#include "SIMModelManager.h"

<<<<<<< HEAD
/************************************************************************
	Constructor / Destructor
************************************************************************/
SIMModelManager::SIMModelManager(void)
{
	init();
}

SIMModelManager::~SIMModelManager(void)
{
	release();
}

void
SIMModelManager::init()
{
	setUserName(_T("SIMModelManager"));

	// by contract
	mec = new MECComponent;
	mec->setUser(this);
}

void
SIMModelManager::release()
{
	meb = nullptr;
	delete mec;
	mec = nullptr;
}

/************************************************************************
	Inherit Function
************************************************************************/
shared_ptr<NOM>
SIMModelManager::registerMsg(tstring msgName)
{
	shared_ptr<NOM> nomMsg = mec->registerMsg(msgName);
	registeredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));

	return nomMsg;
}

void
SIMModelManager::discoverMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));
}

void
SIMModelManager::updateMsg(shared_ptr<NOM> nomMsg)
{
	mec->updateMsg(nomMsg);
}

void
SIMModelManager::reflectMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
}

void
SIMModelManager::deleteMsg(shared_ptr<NOM> nomMsg)
{
	mec->deleteMsg(nomMsg);
	registeredMsg.erase(nomMsg->getInstanceID());
}

void
SIMModelManager::removeMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.erase(nomMsg->getInstanceID());
}

void
SIMModelManager::sendMsg(shared_ptr<NOM> nomMsg)
{
	mec->sendMsg(nomMsg);
}

void
SIMModelManager::recvMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
}

void
SIMModelManager::setUserName(tstring userName)
{
	name = userName;
}

tstring
SIMModelManager::getUserName()
{
	return name;
}

void
SIMModelManager::setData(void* data)
{
	// if need be, write your code
}

bool
SIMModelManager::start()
{
	// if need be, write your code
	airThreat = new SIMModel;
	airThreat->setThreatPosition(1.0, 1.0);
	airThreat->setThreatTargetPosition(10.0, 10.0);

	// binding message
	function<void(shared_ptr<NOM>)> msgProcessor;
	msgProcessor = bind(&SIMModelManager::processStartMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("Start"), msgProcessor));

	return true;
}

bool
SIMModelManager::stop()
{
	// if need be, write your code
	delete airThreat;

	return true;
}

void
SIMModelManager::setMEBComponent(IMEBComponent* realMEB)
{
	meb = realMEB;
	mec->setMEB(meb);
}

/************************************************************************
	message processor
************************************************************************/
void
SIMModelManager::processStartMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
}

/************************************************************************
	Export Function
************************************************************************/
extern "C" BASEMGRDLL_API
BaseManager * createObject()
{
	return new SIMModelManager;
}

extern "C" BASEMGRDLL_API
void deleteObject(BaseManager * userManager)
{
	delete userManager;
}

=======
void SIMModelManager::scnDeploy(shared_ptr<NOM> nomMsg) {
  delete missile;
  delete mData;
  mData = new SharedData();
  missile = new SIMModel(mData);

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
  missile->updateMissilePosition();

  tcout << "msl x : " << mData->x << " msl y : " << mData->y << "\n";
  tcout << "ats x : " << x << " ats y : " << y << "\n";

  notifyMissileInfo();
  //notifyStatus();
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
>>>>>>> dev
