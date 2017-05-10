#ifndef _MGT_APP_H
#define _MGT_APP_H

#include "Platforms/Interfaces/IConnection.h"
#include "Common/Application/porthandler.h"
#include "Common/Application/ttask.h"

#include <list>
#include <map>

class TMgtApplication{
protected:

  WORD ListeningPort;
  TNetworkHostParam Way4Address;
  DWORD dwFirstTID;
  DWORD dwLastTID;
  BYTE* TidStatusArray;
  BOOLEAN bStopFlag;

  std::map<DWORD, TMgtPortHandler*> ActivePorts;
  std::list<TMgtTask*>              ActiveTasks;

  void CheckRequestsOnActivePorts();
  void CheckActiveTasks();

  void AddNewPortHandler(TMgtPortHandler* p);

  TMgtPortHandler* FindHandlerByID(DWORD id)
  {
    TMgtPortHandler* p = ActivePorts[id];
    if(!p)
      ActivePorts.erase(id);
    return p;
  };

public:
  TMgtApplication();
  ~TMgtApplication();
  
  void Run(); 
};

#endif
