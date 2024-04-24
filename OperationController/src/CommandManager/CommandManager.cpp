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
	if (nomMsg->getName() == _T("SIM_CONTROL"))
	{
          unsigned int OpType = nomMsg->getValue(_T("OperationType"))->toUInt();
          if (OpType == static_cast<int>(OP_TYPE::START_SIM))
		  {
				if (!isRunning)
				{
					isRunning = true;
					this->startSimulation();
				}
		  } 
		  else
		  {
				this->stopSimulation();
		  }
        } 
	else if (nomMsg->getName() == _T("MSL_LAUNCH"))
	{
          missileLaunched = true;
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
    missileLaunched = false;
    simReqMsgATS_ROS = meb->getNOMInstance(name, _T("SIM_CONTROL_ATS_ROS"));
    simReqMsgATS_ROS->setValue(_T("MessageId"), &NUInteger(1002));
    simReqMsgATS_ROS->setValue(_T("MessageSize"), &NUInteger(12));
    simReqMsgATS_ROS->setValue(_T("OperationType"), &NUInteger(2));
    simReqMsgMSL_LOS = meb->getNOMInstance(name, _T("SIM_CONTROL_MSL_LOS"));
    simReqMsgMSL_LOS->setValue(_T("MessageId"), &NUInteger(1002));
    simReqMsgMSL_LOS->setValue(_T("MessageSize"), &NUInteger(12));
    simReqMsgMSL_LOS->setValue(_T("OperationType"), &NUInteger(2));
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
	timerHandle = 0;
    timerHandle = nTimer->addPeriodicTask(1000, periodicFunc);
}

void 
CommandManager::stopSimulation() 
{
	// if need be, write your code
	isRunning = false;
    missileLaunched = false;
	nTimer->removeTask(timerHandle);
}


void 
CommandManager::sendPeriodically()
{ 
    this->sendMsg(simReqMsgATS_ROS);
	/*if (!missileLaunched) return;
	this->sendMsg(simReqMsgMSL_LOS);*/
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

