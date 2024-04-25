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
  mRadar->detectEnemy();

  bool istargetdetect = mData->isTargetDetect;
  auto _nomMsg = meb->getNOMInstance(name, _T("ROS_DETECTION"));

  _nomMsg->setValue(_T("MessageId"), &NUInteger(1004));
  _nomMsg->setValue(_T("MessageSize"), &NUInteger(12));
  _nomMsg->setValue(_T("IsDetected"), &NUInteger((int)istargetdetect));

  this->sendMsg(_nomMsg);  // 현재 탐지 여부 전~송
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
>>>>>>> dev
