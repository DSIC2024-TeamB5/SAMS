#include "CommMessageHandler.h"

/************************************************************************
        initialize / release
************************************************************************/
CommMessageHandler::CommMessageHandler() { initialize(); }

CommMessageHandler::~CommMessageHandler() { release(); }

void CommMessageHandler::initialize() {
  // if need be, write your code
  // for read
  setIDNameTable(100, _T("MessageName"));
  setIDNameTable(1001, _T("SCN_DEPLOY"));
  setIDNameTable(1002, _T("SIM_CONTROL"));
  setIDNameTable(1005, _T("MSL_STATUS"));
  setIDNameTable(1008, _T("ATS_POSITION"));
  // for read

  // for write
  setIDNameTable(1003, _T("SIM_STATUS"));
  setIDNameTable(1006, _T("MSL_POSITION"));
  // for write
}

void CommMessageHandler::release() { IDNameTable.clear(); }

/************************************************************************
        ID_Name table management
************************************************************************/
void CommMessageHandler::setIDNameTable(unsigned int msgID, tstring msgName) {
  IDNameTable.insert(pair<unsigned int, tstring>(msgID, msgName));
}

tstring CommMessageHandler::getMsgName(unsigned int msgID) {
  tstring msgName;
  if (auto itr = IDNameTable.find(msgID); itr != IDNameTable.end()) {
    msgName = itr->second;
  } else {
    msgName = _T("");
  }

  return msgName;
}