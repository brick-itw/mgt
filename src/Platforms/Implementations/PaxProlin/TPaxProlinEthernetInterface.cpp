#include "Platforms/Implementations/PaxProlin/TPaxProlinEthernetInterface.h"
#include "Platforms/Implementations/PC/lin/TLinError.h"
#include "Common/Utils/FormatConverters.h"
#include "Base/Types/sub_error.h"
#include <errno.h>
#include <sys/socket.h>
#include <osal.h>

Error TPaxProlinEthernetInterface::SetSelfParams(const TNetworkClientParam& self_ip)
{
  int res = OsNetSetConfig(NET_LINK_ETH, IpToString(self_ip.dwAddress).c_str(), IpToString(self_ip.dwMask).c_str(), IpToString(self_ip.dwGateway).c_str(), NULL);
  if(res == RET_OK)
    return SUCCESS;
  return -1;//todo
}

Error TPaxProlinEthernetInterface::GetSelfParams(TNetworkClientParam& self_ip)
{
  char ip[100];
  char mask[100];
  char gateway[100];
  memset(ip, 0, sizeof(ip));
  memset(mask, 0, sizeof(mask));
  memset(gateway, 0, sizeof(gateway));

  int res = OsNetGetConfig(NET_LINK_ETH, ip, mask, gateway, NULL);
  if(res == RET_OK)
  {
    self_ip.dwAddress = StringToIp(ip);
    self_ip.dwMask = StringToIp(mask);
    self_ip.dwGateway = StringToIp(gateway);
    return SUCCESS;
  }
  return -1;//todo
}


