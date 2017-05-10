#ifndef _MGT_TASK_H
#define _MGT_TASK_H

#include "Common/Utils/T_Trivial_TLV.h"
#include "Base/Types/modules.h"

#define MGT_TASK_ERROR(code) REGISTER_ERROR(MODULE_MGT_TASK, (code))

enum MgtTaskErrorCode
{
  ERROR_PAN_DECRYPTION_FAILED  = MGT_TASK_ERROR(1),
};

enum MGT_TASK_STATUS{
  TS_JUST_RECEIVED,
  TS_SENT_TO_HOST,
  TS_APPROVED,
  TS_DECLINED,
  TS_NEED_EXTRA_AUTH,
  TS_REVERSAL_STARTED,
  TS_REVERSAL_DONE,
  TS_ERROR,
  TS_DONE
};

#define TMM_HOST_REPLY_TIMEOUT  45000UL

class TMgtTask {
protected:
  ITimer*              pTimer;
  DWORD                ID;
  DWORD                dwPortHandlerID;
  T_Trivial_TLV_Buffer Request;
  MGT_TASK_STATUS      Status;
  DWORD                ResultCode;
  IConnection*         pWay4Link;
  DWORD                TID;

  void SetTimer(DWORD msec)
  {
    delete pTimer;
    pTimer = ISystem::Instance()->CreateTimer(msec);
  };


public:
  TMgtTask (DWORD ph_id, const T_Trivial_TLV_Buffer& request, DWORD way4_tid, const TNetworkHostParam& host);
  ~TMgtTask();

  DWORD GetPortHandlerID() const {return dwPortHandlerID;};
  DWORD GetTaskID() const {return ID;};

  void Process();


  BOOLEAN NeedsReversal()      const {return (Status == TS_SENT_TO_HOST) || (Status==TS_APPROVED) || (Status==TS_REVERSAL_STARTED);};
  BOOLEAN ReversalInProgress() const {return (Status == TS_REVERSAL_STARTED);};
  BOOLEAN Complete()           const {return (Status == TS_DONE) || (Status == TS_ERROR);};
  BOOLEAN ReadyToDeliver()     const {return (Status == TS_APPROVED) || (Status == TS_DECLINED);};
  BOOLEAN Expired()            const {return pTimer->IsExpired();};

};

#endif
