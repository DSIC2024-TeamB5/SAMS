#include "SIMModelManager.h"

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
SIMModelManager::recvMsg(shared_ptr<NOM> nomMsg) // 버스에 올라간(퍼블리시된) 데이터가 생기면 SIM이 구독(Subscribe?)라서 바로 가져오는듯
{												// 그 부분이 여기
	// if need be, write your code
	if (nomMsg->getName() == _T("ScenarioDeploy")) {
		
	}
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
	//radarScenario = new SIMModel;
	//radarScenario->setPosition(1.0, 1.0);
	//airThreat->setThreatTargetPosition(10.0, 10.0);

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
	delete radarScenario;

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

