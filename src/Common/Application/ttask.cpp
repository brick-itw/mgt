#include "Common/Application/ttask.h"
#include "Common/Loggers/LogEvents.h"
#include "Common/Application//mgt_tags.h"
#include "Common/Application/mgt_errors.h"
#include "Common/Application/tsystem.h"

static DWORD dwTaskID = 0;

TMgtTask::TMgtTask (DWORD ph_id, const T_Trivial_TLV_Buffer& request) : 
          pTimer(NULL),
          dwPortHandlerID(ph_id), ID(++dwTaskID), Request(request), Status(TS_JUST_RECEIVED), ResultCode(MGTERR_NOT_PROCESSED),
          pWay4Link(NULL), TID(way4_tid)
{
  TSystem::Instance().LogData(TMgtTaskCreated(ID, dwPortHandlerID, 
                                                  Request[TAG_MGT_AMOUNT].AsBinary().MsbDword(),
                                                  Request[TAG_MGT_REQ_TYPE].AsInteger(),
                                                  Request[TAG_MGT_REQ_ID].AsBcdString()
                                                  ));

  if(!DecryptTagData())
  {
    ResultCode = MGTERR_DECRYPTION_FAILED;
    State = TS_ERROR;
  }
  else
  {
    IEthernetInterface* pIface = (IEthernetInterface*)ISystem::Instance()->GetDeviceLayer()->GetDevice(DVC_ETHERNET, 0);
    pIface->Call(host, &pWay4Link);
    pTimer = ISystem::Instance()->CreateTimer(TMM_HOST_REPLY_TIMEOUT);
  }
}

TMgtTask::~TMgtTask()
{
  TSystem::Instance().LogData(TMgtTaskDestroyed(ID, ResultCode)); 
  if(pWay4Link)
    delete pWay4Link;
  if(pTimer)
    delete pTimer;
}

void TMgtTask::Process()
{
}
