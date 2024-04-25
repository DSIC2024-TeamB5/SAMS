#include "UDPManager.h"

/************************************************************************
<<<<<<< HEAD
	Constructor / Destructor
************************************************************************/
UDPManager::UDPManager(void)
{
	init();
}

UDPManager::~UDPManager(void)
{
	release();
}

/************************************************************************
	initialize / release
************************************************************************/
void
UDPManager::init()
{
	setUserName(_T("UDPManager"));

	// by contract
	mec = new MECComponent;
	mec->setUser(this);

	commConfig = new CommunicationConfig;
	commConfig->setIni(_T("CommLinkInfo.ini"));
	commInterface = new NCommInterface(this);
	tcout << _T("UDP Communication") << endl;
}

void
UDPManager::release()
{
	delete commConfig;
	delete commInterface;

	meb = nullptr;
	delete mec;
	mec = nullptr;
}

/************************************************************************
	Inherit Function
************************************************************************/
shared_ptr<NOM>
UDPManager::registerMsg(tstring msgName)
{
	shared_ptr<NOM> nomMsg = mec->registerMsg(msgName);
	registeredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));

	return nomMsg;
}

void
UDPManager::discoverMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));
}

void
UDPManager::updateMsg(shared_ptr<NOM> nomMsg)
{
	mec->updateMsg(nomMsg);
}

void
UDPManager::reflectMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code

	commInterface->sendCommMsg(nomMsg);
}

void
UDPManager::deleteMsg(shared_ptr<NOM> nomMsg)
{
	mec->deleteMsg(nomMsg);
	registeredMsg.erase(nomMsg->getInstanceID());
}

void
UDPManager::removeMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.erase(nomMsg->getInstanceID());
}

void
UDPManager::sendMsg(shared_ptr<NOM> nomMsg)
{
	mec->sendMsg(nomMsg);
}

void
UDPManager::recvMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code

	commInterface->sendCommMsg(nomMsg);
}

void
UDPManager::setUserName(tstring userName)
{
	name = userName; 
}

tstring
UDPManager::getUserName()
{
	return name;
}

void
UDPManager::setData(void* data)
{
	// if need be, write your code
}

bool
UDPManager::start()
{
	commInterface->setMEBComponent(meb);
	MessageProcessor msgProcessor = bind(&UDPManager::processRecvMessage, this, placeholders::_1, placeholders::_2);
	commConfig->setMsgProcessor(msgProcessor);
	commInterface->initNetEnv(commConfig);

	return true;
}

bool
UDPManager::stop()
{
	commInterface->releaseNetEnv(commConfig);
	return true;
}

void
UDPManager::setMEBComponent(IMEBComponent* realMEB)
{
	meb = realMEB;
	mec->setMEB(meb);
}

void
UDPManager::processRecvMessage(unsigned char* data, int size)
{
	auto IDSize = commConfig->getHeaderIDSize();
	auto IDPos = commConfig->getHeaderIDPos();

	unsigned int tmpMsgID;
	memcpy(&tmpMsgID, data + IDPos, IDSize);
	auto msgID = ntohl(tmpMsgID);

	auto nomMsg = meb->getNOMInstance(name, commMsgHandler.getMsgName(msgID));

	if (nomMsg.get())
	{
		if (nomMsg->getType() == NOM_TYPE_OBJECT)
		{
			nomMsg->deserialize(data, size);
			this->updateMsg(nomMsg);
		}
		else
		{
			auto nomMsgCP = nomMsg->clone();
			nomMsgCP->deserialize(data, size);
			this->sendMsg(nomMsgCP);
		}
	}
	else
	{
		tcerr << _T("not defined message") << endl;
	}
}

/************************************************************************
	Export Function
************************************************************************/
extern "C" BASEMGRDLL_API
BaseManager* createObject()
{
	return new UDPManager;
}

extern "C" BASEMGRDLL_API
void deleteObject(BaseManager* userManager)
{
	delete userManager;
}

=======
        Constructor / Destructor
************************************************************************/
UDPManager::UDPManager(void) { init(); }

UDPManager::~UDPManager(void) { release(); }

/************************************************************************
        initialize / release
************************************************************************/
void UDPManager::init() {
  setUserName(_T("UDPManager"));

  // by contract
  mec = new MECComponent;
  mec->setUser(this);

  commConfig = new CommunicationConfig;
  commConfig->setIni(_T("CommLinkInfo.ini"));
  commInterface = new NCommInterface(this);
  tcout << _T("UDP Communication") << endl;
}

void UDPManager::release() {
  delete commConfig;
  delete commInterface;

  meb = nullptr;
  delete mec;
  mec = nullptr;
}

/************************************************************************
        Inherit Function
************************************************************************/
shared_ptr<NOM> UDPManager::registerMsg(tstring msgName) {
  shared_ptr<NOM> nomMsg = mec->registerMsg(msgName);
  registeredMsg.insert(
      pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));

  return nomMsg;
}

void UDPManager::discoverMsg(shared_ptr<NOM> nomMsg) {
  discoveredMsg.insert(
      pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));
}

void UDPManager::updateMsg(shared_ptr<NOM> nomMsg) { mec->updateMsg(nomMsg); }

void UDPManager::reflectMsg(shared_ptr<NOM> nomMsg) {
  // if need be, write your code
  commInterface->sendCommMsg(nomMsg);
}

void UDPManager::deleteMsg(shared_ptr<NOM> nomMsg) {
  mec->deleteMsg(nomMsg);
  registeredMsg.erase(nomMsg->getInstanceID());
}

void UDPManager::removeMsg(shared_ptr<NOM> nomMsg) {
  discoveredMsg.erase(nomMsg->getInstanceID());
}

void UDPManager::sendMsg(shared_ptr<NOM> nomMsg) { mec->sendMsg(nomMsg); }

void UDPManager::recvMsg(shared_ptr<NOM> nomMsg) {
  // if need be, write your code

  commInterface->sendCommMsg(nomMsg);
}

void UDPManager::setUserName(tstring userName) { name = userName; }

tstring UDPManager::getUserName() { return name; }

void UDPManager::setData(void *data) {
  // if need be, write your code
}

bool UDPManager::start() {
  commInterface->setMEBComponent(meb);
  MessageProcessor msgProcessor = bind(&UDPManager::processRecvMessage, this,
                                       placeholders::_1, placeholders::_2);
  commConfig->setMsgProcessor(msgProcessor);
  commInterface->initNetEnv(commConfig);

  return true;
}

bool UDPManager::stop() {
  commInterface->releaseNetEnv(commConfig);
  return true;
}

void UDPManager::setMEBComponent(IMEBComponent *realMEB) {
  meb = realMEB;
  mec->setMEB(meb);
}

void UDPManager::processRecvMessage(unsigned char *data, int size) {
  auto IDSize = commConfig->getHeaderIDSize();
  auto IDPos = commConfig->getHeaderIDPos();

  unsigned int tmpMsgID;
  memcpy(&tmpMsgID, data + IDPos, IDSize);

  auto msgID = ntohl(tmpMsgID);

  auto nomMsg = meb->getNOMInstance(name, commMsgHandler.getMsgName(msgID));

  if (nomMsg.get()) {
    if (nomMsg->getType() == NOM_TYPE_OBJECT) {
      nomMsg->deserialize(data, size);
      this->updateMsg(nomMsg);
    } else {
      auto nomMsgCP = nomMsg->clone();
      nomMsgCP->deserialize(data, size);
      this->sendMsg(nomMsgCP);
    }
  } else {
    tcerr << _T("not defined message") << endl;
  }
}

/************************************************************************
        Export Function
************************************************************************/
extern "C" BASEMGRDLL_API BaseManager *createObject() { return new UDPManager; }

extern "C" BASEMGRDLL_API void deleteObject(BaseManager *userManager) {
  delete userManager;
}
>>>>>>> dev
