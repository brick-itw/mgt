#ifndef GPRS_INTERFACE_H
#define GPRS_INTERFACE_H

#include "Base/Types/inttypes.h"
#include "Platforms/Interfaces/IConnection.h"
#include "Platforms/Implementations/PC/lin/TLinEthernetInterface.h"

class TPaxProlinGprsInterface : /*public IGprsInterface, */public TLinEthernetInterface
{
public:
	TPaxProlinGprsInterface(int simID);
	virtual ~TPaxProlinGprsInterface();

	virtual Error Open();
	virtual void Close();

  virtual NWINTSTATE GetCurrentState();
  virtual Error      GetNetworkError();

/*
	virtual Error SetupPppParams   (const TPppParams& ppp)
	{
	  PppParams = ppp;
	  return SUCCESS;
	};

	virtual Error SetupDialupParams(const TDialupParams& dial)
	{
	  DialupParams = dial;
    return SUCCESS;
	};

	virtual BYTE GetSignalLevelOutOf5();
	virtual const char* GetOperatorName();
*/


protected:

	Error          GetPaxGprsErrorMeaning(int err);

	NWINTSTATE     NwState;
  int            SimID;  //@TODO
	TPppParams     PppParams;
	TDialupParams  DialupParams;
	Error          InternalNetworkError;
};

#endif
