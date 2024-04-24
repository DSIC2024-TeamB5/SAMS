#include "CommMessageHandler.h"

/************************************************************************
        initialize / release
************************************************************************/
CommMessageHandler::CommMessageHandler() { initialize(); }

CommMessageHandler::~CommMessageHandler() { release(); }

void CommMessageHandler::initialize() {
  // if need be, write your code
  setIDNameTable(1001, _T("SCN_DEPLOY"));
  setIDNameTable(1002, _T("SIM_CONTROL"));
  setIDNameTable(1008, _T("ATS_POSITION"));
}

void CommMessageHandler::release() { IDNameTable.clear(); }

/************************************************************************
        ID_Name table management
************************************************************************/
void CommMessageHandler::setIDNameTable(unsigned short msgID, tstring msgName) {
  IDNameTable.insert(pair<unsigned short, tstring>(msgID, msgName));
}

tstring CommMessageHandler::getMsgName(unsigned short msgID) {
  tstring msgName;
  if (auto itr = IDNameTable.find(msgID); itr != IDNameTable.end()) {
    msgName = itr->second;
  } else {
    msgName = _T("");
  }

  return msgName;
}