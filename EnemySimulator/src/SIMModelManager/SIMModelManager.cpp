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
  delete enemySim;
  delete mData;
  mData = new SharedData;
  enemySim = new EnemySimulation(mData);

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
>>>>>>> dev
