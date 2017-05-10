#ifndef _LIN_ETHERNET_PAX_H
#define _LIN_ETHERNET_PAX_H

#include "Platforms/Interfaces/IConnection.h"
//#include "Common/Loggers/TConnectionLogger.h"
#include <sys/socket.h>
#include "Platforms/Implementations/pc/TSocketInterface.h"
#include "Platforms/Implementations/pc/lin/TLinEthernetInterface.h"

class TPaxProlinEthernetInterface : public TLinEthernetInterface {
protected:

public:
  TPaxProlinEthernetInterface(IFileSystem* ptraces = NULL)
                      : TLinEthernetInterface(ptraces)
  {
  };

  virtual Error SetSelfParams(const TNetworkClientParam& self_ip);
  virtual Error GetSelfParams(TNetworkClientParam& self_ip);

};



#endif
