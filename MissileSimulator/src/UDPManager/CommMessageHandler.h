#pragma once
<<<<<<< HEAD
#include <map>
#include <nFramework/util/util.h>

class CommMessageHandler
{
public:
	CommMessageHandler();
	~CommMessageHandler();

	tstring getMsgName(unsigned int msgID);

private:
	void initialize();
	void release();
	void setIDNameTable(unsigned int msgID, tstring msgName);

private:
	map<unsigned int, tstring> IDNameTable;
};

=======
#include <nFramework/util/util.h>

#include <map>

class CommMessageHandler {
 public:
  CommMessageHandler();
  ~CommMessageHandler();

  tstring getMsgName(unsigned int msgID);

 private:
  void initialize();
  void release();
  void setIDNameTable(unsigned int msgID, tstring msgName);

 private:
  map<unsigned int, tstring> IDNameTable;
};
>>>>>>> dev
