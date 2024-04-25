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
>>>>>>> dev
