#ifndef _MGT_TASK_H
#define _MGT_TASK_H

#include "Common/Utils/T_Trivial_TLV.h"
#include "Platforms/Interfaces/ISystem.h"

enum MGT_TASK_STATUS{
  TS_JUST_RECEIVED,
  TS_SENT_TO_HOST,
  TS_APPROVED,
  TS_DECLINED,
  TS_NEED_EXTRA_AUTH,
  TS_REVERSAL_STARTED,
  TS_REVERSAL_DONE,
  TS_DONE
};

#define TMM_HOST_REPLY_TIMEOUT  45000UL

class TMgtTask {
protected:
  ITimer*              pTimer;
  DWORD                dwHandlerID;
  T_Trivial_TLV_Buffer Request;
  MGT_TASK_STATUS      Status;

public:
  TMgtTask (DWORD id, const T_Trivial_TLV_Buffer& request) : dwHandlerID(id), Request(request), Status(TS_JUST_RECEIVED)
  {
    pTimer = ISystem::Instance()->CreateTimer(TMM_HOST_REPLY_TIMEOUT);
  };
  ~TMgtTask()
  {
    delete pTimer;
  };
  void SetStatus(MGT_TASK_STATUS st) {Status = st;};
  const T_Trivial_TLV_Buffer& Data() const {return Request;};
  DWORD GetID() const {return dwHandlerID;};
  void SetTimer(DWORD msec)
  {
    delete pTimer;
    pTimer = ISystem::Instance()->CreateTimer(msec);
  };

  BOOLEAN JustArrived()        const {return  Status == TS_JUST_RECEIVED;};
  BOOLEAN NeedsReversal()      const {return (Status == TS_SENT_TO_HOST) || (Status==TS_APPROVED) || (Status==TS_REVERSAL_STARTED);};
  BOOLEAN ReversalInProgress() const {return (Status == TS_REVERSAL_STARTED);};
  BOOLEAN Complete()           const {return  Status == TS_DONE;};
  BOOLEAN ReadyToDeliver()     const {return (Status == TS_APPROVED) || (Status == TS_DECLINED);};
  BOOLEAN Expired()            const {return pTimer->IsExpired();};

};

#endif
