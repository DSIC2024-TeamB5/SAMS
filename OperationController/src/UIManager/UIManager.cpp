#include "UIManager.h"
#include "UIMessageEnum.h"
#include <cassert>

/************************************************************************
	constructor / destructor
************************************************************************/
UIManager::UIManager(void)
{
	initialize();
}

UIManager::~UIManager(void)
{
	release();
}

/************************************************************************
	initialize / release
************************************************************************/
void
UIManager::initialize(void)
{
	setUserName(_T("UIManager"));

	// design by contract
	mec = new MECComponent;
	mec->setUser(this);
}

void
UIManager::release()
{
	meb = NULL;
	delete mec;
	mec = NULL;
}

/************************************************************************
	inherited function
************************************************************************/
shared_ptr<NOM>
UIManager::registerMsg(tstring msgName)
{
	shared_ptr<NOM> nomMsg = mec->registerMsg(msgName);
	registeredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));

	return nomMsg;
}

void
UIManager::discoverMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));
	::SendMessage(winHandle, UM_DiscoveredNOM, (WPARAM)nomMsg.get(), 0);
}

void
UIManager::updateMsg(shared_ptr<NOM> nomMsg)
{
	// you can use the code below, if necessary
	nomMsg->copyTo(icdTestNom);
	mec->updateMsg(icdTestNom);
}

void
UIManager::reflectMsg(shared_ptr<NOM> nomMsg)
{
	// you can use the code below, if necessary
	if (nomMsg->getName() == _T("ICD_Test1"))
	{
		auto length = 0;
		unsigned char* nomBytes = nomMsg->serialize(length);

		NOMInfo nomInfo;
		_tcscpy(nomInfo.MsgName, nomMsg->getName().c_str());
		nomInfo.MsgID = nomMsg->getMessageID();
		nomInfo.MsgLen = length;

		::SendMessage(winHandle, UM_ReflectedNOM, (WPARAM)&nomInfo, (LPARAM)nomBytes);
		delete[] nomBytes;
	}
}

void
UIManager::deleteMsg(shared_ptr<NOM> nomMsg)
{
	registeredMsg.erase(nomMsg->getInstanceID());
	mec->deleteMsg(nomMsg);
}

void
UIManager::removeMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.erase(nomMsg->getInstanceID());
	::SendMessage(winHandle, UM_RemovedNOM, (WPARAM)nomMsg.get(), 0);
}

void
UIManager::sendMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
		//if (nomMsg->getName() == _T("SIM_CONTROL")) {
  //        auto _nomMsg = meb->getNOMInstance(name, _T("SIM_CONTROL"));
  //        _nomMsg->setValue(_T("MessageId"), &NUInteger(1002));
  //        _nomMsg->setValue(_T("MessageSize"), &NUInteger(12));
  //        _nomMsg->setValue(_T("OperationType"), &NUInteger(1));
  //        mec->sendMsg(_nomMsg);
	 // }
	//else
	mec->sendMsg(nomMsg);
}

void
UIManager::recvMsg(shared_ptr<NOM> nomMsg)
{
	// you can use the code below, if necessary
	// shared_ptr<NOM> guiNOM = uiMessageHadler->getGUIMsgNOM(nomMsg);
	auto length = 0;
	unsigned char *nomBytes = nomMsg->serialize(length);

	NOMInfo nomInfo;
	_tcscpy(nomInfo.MsgName, nomMsg->getName().c_str());
	nomInfo.MsgID = nomMsg->getMessageID();
	nomInfo.MsgLen = length;

	::SendMessage(winHandle, UM_ReceivedNOM, (WPARAM)&nomInfo, (LPARAM)nomBytes);
	delete[] nomBytes;
	
	//::SendMessage(winHandle, UM_ReceivedNOM, (WPARAM)nomMsg.get(), 0);
}

void
UIManager::setUserName(tstring userName)
{
	name = userName;
}

tstring
UIManager::getUserName()
{
	return name;
}

void
UIManager::setData(void* data)
{
	this->winHandle = (HWND)data;
	assert("HWND == 0" && winHandle);
}

bool
UIManager::start()
{
	// if need be, write your code

	return true;
}

bool
UIManager::stop()
{
	// if need be, write your code

	return true;
}

void
UIManager::setMEBComponent(IMEBComponent* realMEB)
{
	meb = realMEB;
	mec->setMEB(meb);
}

/************************************************************************
	exported function
************************************************************************/
extern "C" BASEMGRDLL_API
BaseManager * createObject()
{
	return new UIManager;
}

extern "C" BASEMGRDLL_API
void deleteObject(BaseManager * userManager)
{
	delete userManager;
}