#include "Platforms/Implementations/PaxProlin/TPaxProlinGprsInterface.h"
#include "Common/Application/TTerminalEnvironment.h"
#include "Base/Types/error.h"

#include <osal.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/socket.h>
#include "netinet/in.h"
#include "arpa/inet.h"



TPaxProlinGprsInterface::TPaxProlinGprsInterface(int simID) :
                   TLinEthernetInterface(NULL), NwState(NWS_DOWN),
                   SimID(simID),
                   InternalNetworkError(ERROR_NET_INTERFACE_DOWN)
{
  DialupParams = TTerminalEnvironment::Instance().GetDialupParams();
  PppParams    = TTerminalEnvironment::Instance().GetPppParams();
}

TPaxProlinGprsInterface::~TPaxProlinGprsInterface()
{
}

Error TPaxProlinGprsInterface::GetNetworkError()
{
  return InternalNetworkError;
}

Error TPaxProlinGprsInterface::GetPaxGprsErrorMeaning(int err)
{
  switch(err) //@TODO
  {
     case -212: return ERROR_GPRS_LOGIN_FAILED;
     default:   return ERROR_NET_INTERFACE_DOWN;
  }
}


NWINTSTATE TPaxProlinGprsInterface::GetCurrentState()
{
  if(NwState == NWS_ACCESSING_MEDIA)
  {
    int err = OsWlCheck();
    if(!err)
      NwState = NWS_UP;
    else
    if(err != PPP_LOGINING)
    {
      InternalNetworkError = GetPaxGprsErrorMeaning(err);
      NwState = NWS_ERROR;
    }
  }
  return NwState;
}


Error TPaxProlinGprsInterface::Open()
{
  if((NwState == NWS_UP) || (NwState == NWS_ACCESSING_MEDIA))
    return SUCCESS;

  if(!DialupParams.InitStr[0])
    return ERROR_GPRS_NO_APN;

  int err;
  //OsWlSwitchPower(1);
  err = OsWlLock();
  err = OsWlSelSim(SimID);     //default 0
  err = OsWlInit(0);  //default 0 , 0 means no need password
  err = OsWlLogin( DialupParams.InitStr,     // APN
                     PppParams.Login,          // Name,
                     PppParams.Pwd,            // Password,
                     PPP_ALG_ALL,              // Auth,
                     0, //3
                     30000,
                     0);
  if(err == PPP_LOGINING)
  {
    NwState = NWS_ACCESSING_MEDIA;
  }
  else
  if(err == 0)
  {
    NwState = NWS_UP;
    return SUCCESS;
  }
  else
  {
    InternalNetworkError = GetPaxGprsErrorMeaning(err);
    NwState = NWS_ERROR;
  }

  if(NwState == NWS_ERROR)
  {
    OsWlLogout();
    OsWlUnLock();
  };

  return InternalNetworkError;
}

void TPaxProlinGprsInterface::Close()
{
	OsWlLogout();
	OsWlUnLock();
	//OsWlSwitchPower(0);
	NwState = NWS_DOWN;
  InternalNetworkError = ERROR_NET_INTERFACE_DOWN;
}

/*
BYTE TPaxProlinGprsInterface::GetSignalLevelOutOf5()
{
  return 0;//not implemented
}


const char* TPaxProlinGprsInterface::GetOperatorName()
{
	return NULL;
}
*/

