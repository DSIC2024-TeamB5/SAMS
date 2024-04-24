#include "CommandManager.h"

/************************************************************************
	Constructor / Destructor
************************************************************************/
CommandManager::CommandManager(void)
{
	init();
}

CommandManager::~CommandManager(void)
{
	release();
}

void
CommandManager::init()
{
	setUserName(_T("CommandManager"));

	// by contract
	mec = new MECComponent;
	mec->setUser(this);
}

void
CommandManager::release()
{
	meb = nullptr;
	delete mec;
	mec = nullptr;
}

/************************************************************************
	Inherit Function
************************************************************************/
shared_ptr<NOM>
CommandManager::registerMsg(tstring msgName)
{
	shared_ptr<NOM> nomMsg = mec->registerMsg(msgName);
	registeredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));

	return nomMsg;
}

void
CommandManager::discoverMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));
}

void
CommandManager::updateMsg(shared_ptr<NOM> nomMsg)
{
	mec->updateMsg(nomMsg);
}

void
CommandManager::reflectMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
}

void
CommandManager::deleteMsg(shared_ptr<NOM> nomMsg)
{
	mec->deleteMsg(nomMsg);
	registeredMsg.erase(nomMsg->getInstanceID());
}

void
CommandManager::removeMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.erase(nomMsg->getInstanceID());
}

void
CommandManager::sendMsg(shared_ptr<NOM> nomMsg)
{
	mec->sendMsg(nomMsg);
}

void
CommandManager::recvMsg(shared_ptr<NOM> nomMsg)
{
	// you can change the code below, if necessary
	if (nomMsg->getName() == _T("MSL_LAUNCH"))
	{
          if (!isRunning)
		  {
            isRunning = true;
			this->startSimulation();
		  }
	}

	// if need be, write your code
}

void
CommandManager::setUserName(tstring userName)
{
	name = userName;
}

tstring
CommandManager::getUserName()
{
	return name;
}

void
CommandManager::setData(void* data)
{
	// if need be, write your code
}

bool
CommandManager::start()
{
	// you can change the code below, if necessary

	nTimer = &NTimer::getInstance();
    isRunning = false;
    simReqMsg = meb->getNOMInstance(name, _T("SIM_CONTROL"));
    simReqMsg->setValue(_T("MessageId"), &NUInteger(1002));
    simReqMsg->setValue(_T("MessageSize"), &NUInteger(12));
    simReqMsg->setValue(_T("OperationType"), &NUInteger(2));

	return true;
}

bool
CommandManager::stop()
{

	this->deleteMsg(ICD_TestNOM);

	return true;
}

void
CommandManager::setMEBComponent(IMEBComponent* realMEB)
{
	meb = realMEB;
	mec->setMEB(meb);
}

/************************************************************************
        message processor
************************************************************************/
void
CommandManager::startSimulation() 
{
	// if need be, write your code
	function<void(void *)> periodicFunc;
    periodicFunc = std::bind(&CommandManager::sendPeriodically, this);
	int timerHandle = 0;
    timerHandle = nTimer->addPeriodicTask(1000, periodicFunc);
}

void 
CommandManager::stopSimulation() 
{
	// if need be, write your code
}


void 
CommandManager::sendPeriodically()
{ 
	
    this->sendMsg(simReqMsg);
	//tcout << _T("HELLO") << endl;
}

/************************************************************************
	Export Function
************************************************************************/
extern "C" BASEMGRDLL_API
BaseManager * createObject()
{
	return new CommandManager;
}

extern "C" BASEMGRDLL_API
void deleteObject(BaseManager * userManager)
{
	delete userManager;
}

