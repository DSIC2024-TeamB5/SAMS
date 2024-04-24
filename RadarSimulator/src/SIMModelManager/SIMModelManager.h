#pragma once
#include <nFramework/BaseManager.h>
#include <nFramework/mec/MECComponent.h>
#include <nFramework/nom/NOMMain.h>

#include "SIMModel.h"

using namespace nframework;
using namespace nom;

class BASEMGRDLL_API SIMModelManager : public BaseManager {
 public:
  SIMModelManager(void);
  ~SIMModelManager(void);
  void scnDeploy(shared_ptr<NOM>);
  void simControl(shared_ptr<NOM>);
  void atsPosition(shared_ptr<NOM>);
  void notifyStatus();

 public:
  // inherited from the BaseManager class
  virtual shared_ptr<NOM> registerMsg(tstring) override;
  virtual void discoverMsg(shared_ptr<NOM>) override;
  virtual void updateMsg(shared_ptr<NOM>) override;
  virtual void reflectMsg(shared_ptr<NOM>) override;
  virtual void deleteMsg(shared_ptr<NOM>) override;
  virtual void removeMsg(shared_ptr<NOM>) override;
  virtual void sendMsg(shared_ptr<NOM>) override;
  virtual void recvMsg(shared_ptr<NOM>) override;
  virtual void setUserName(tstring) override;
  virtual tstring getUserName() override;
  virtual void setData(void*) override;
  virtual bool start() override;
  virtual bool stop() override;
  virtual void setMEBComponent(IMEBComponent*) override;

 private:
  void init();
  void release();

  void processStartMsg(shared_ptr<NOM>);

 private:
  IMEBComponent* meb;
  MECComponent* mec;
  tstring name;
  map<unsigned int, shared_ptr<NOM>> registeredMsg;
  map<unsigned int, shared_ptr<NOM>> discoveredMsg;
  map<tstring, function<void(shared_ptr<NOM>)>> msgMethodMap;

  SharedData* mData;
  SIMModel* mRadar;
};

SIMModelManager::SIMModelManager(void) { init(); }

SIMModelManager::~SIMModelManager(void) { release(); }

void SIMModelManager::init() {
  setUserName(_T("SIMModelManager"));

  // by contract
  mec = new MECComponent;
  mec->setUser(this);
}

void SIMModelManager::release() {
  meb = nullptr;
  delete mec;
  mec = nullptr;
}

/************************************************************************
        Inherit Function
************************************************************************/
shared_ptr<NOM> SIMModelManager::registerMsg(tstring msgName) {
  shared_ptr<NOM> nomMsg = mec->registerMsg(msgName);
  registeredMsg.insert(
      pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));

  return nomMsg;
}

void SIMModelManager::discoverMsg(shared_ptr<NOM> nomMsg) {
  discoveredMsg.insert(
      pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));
}

void SIMModelManager::updateMsg(shared_ptr<NOM> nomMsg) {
  mec->updateMsg(nomMsg);
}

void SIMModelManager::reflectMsg(shared_ptr<NOM> nomMsg) {
  // if need be, write your code
}

void SIMModelManager::deleteMsg(shared_ptr<NOM> nomMsg) {
  mec->deleteMsg(nomMsg);
  registeredMsg.erase(nomMsg->getInstanceID());
}

void SIMModelManager::removeMsg(shared_ptr<NOM> nomMsg) {
  discoveredMsg.erase(nomMsg->getInstanceID());
}

void SIMModelManager::sendMsg(shared_ptr<NOM> nomMsg) { mec->sendMsg(nomMsg); }

void SIMModelManager::recvMsg(shared_ptr<NOM> nomMsg) {
  auto msgMapKey = nomMsg->getName();
  tcout << "message received :: " << msgMapKey << "\n";
  auto it = msgMethodMap.find(msgMapKey);
  it->second(nomMsg);
}

void SIMModelManager::setUserName(tstring userName) { name = userName; }

tstring SIMModelManager::getUserName() { return name; }

void SIMModelManager::setData(void* data) {
  // if need be, write your code
}

void SIMModelManager::setMEBComponent(IMEBComponent* realMEB) {
  meb = realMEB;
  mec->setMEB(meb);
}

void SIMModelManager::processStartMsg(shared_ptr<NOM> nomMsg) {
  // if need be, write your code
}

extern "C" BASEMGRDLL_API BaseManager* createObject() {
  return new SIMModelManager;
}

extern "C" BASEMGRDLL_API void deleteObject(BaseManager* userManager) {
  delete userManager;
}