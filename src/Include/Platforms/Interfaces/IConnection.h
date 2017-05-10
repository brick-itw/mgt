/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Buzzer device interface declaration
 */

#ifndef _U2_CONNECTION_H
#define _U2_CONNECTION_H

#include "Base/Types/types.h"
#include "Base/Types/modules.h"
#include "Platforms/Interfaces/IDevice.h"

#define CONNECTION_ERROR(code) REGISTER_ERROR(MODULE_CONNECTION, (code))

enum ConnectionErrorCode
{
  ERROR_CONNECT_FAILED  = CONNECTION_ERROR(1),
  ERROR_SEND_FAILED     = CONNECTION_ERROR(2),
  ERROR_LINK_BROKEN     = CONNECTION_ERROR(3),
  ERROR_LINK_PROBLEM    = CONNECTION_ERROR(4),
  ERROR_SELF_PAR_FAIL   = CONNECTION_ERROR(5),
  ERROR_SOCKET_CREATION = CONNECTION_ERROR(6),
  ERROR_SOCKET_BIND     = CONNECTION_ERROR(7),
  ERROR_SOCKET_LISTEN   = CONNECTION_ERROR(8),
  ERROR_SOCKET_ACCEPT   = CONNECTION_ERROR(9),
  ERROR_DNS_FAILURE     = CONNECTION_ERROR(10),
  ERROR_NO_NET_INTERFACE      = CONNECTION_ERROR(11),
  ERROR_NET_INTERFACE_DOWN    = CONNECTION_ERROR(12),
  ERROR_GPRS_NO_APN           = CONNECTION_ERROR(13),
  ERROR_GPRS_LOGIN_FAILED     = CONNECTION_ERROR(14),
  ERROR_NET_INTERFACE_TIMEOUT = CONNECTION_ERROR(15),
};


struct TNetworkClientParam {
  DWORD dwAddress;
  DWORD dwMask;
  DWORD dwGateway;
  BOOLEAN bDhcpEnabled; //true, if dhcp enabled. In other case static ip address used. 
 
  TNetworkClientParam(DWORD ip, DWORD mask, DWORD gateway, BOOLEAN bDhcp)
    : dwAddress(ip), dwMask(mask), dwGateway(gateway), bDhcpEnabled(bDhcp)
  {
  };    
};


#define MAX_SSL_NAME_LEN   65
struct TNetworkHostParam {
  DWORD    dwAddress;
  WORD     wPort;
  BOOLEAN  UseSSL;

  char  SslHostCert[MAX_SSL_NAME_LEN];
  char  SslDevFile [MAX_SSL_NAME_LEN];

  TNetworkHostParam(DWORD ip = 0, WORD port = 0,
                BOOLEAN b_use_ssl = 0,
                const char* ssl_host_cert = 0,
                const char* ssl_device_keypair = 0
               )
  {
    dwAddress  = ip;
    wPort      = port;
    UseSSL  = b_use_ssl;

    _safecopy(SslHostCert, ssl_host_cert);
    _safecopy(SslDevFile,  ssl_device_keypair);
  };
};


typedef enum{
  WPT_NONE   = 0,
  WPT_WEP64  = 1,
  WPT_WEP128 = 2,
  WPT_WPA    = 3,
  WPT_WPA2   = 4
}WIFIPROTECTION;

#define MAX_SSID_LEN  65
#define MAX_PWD_LEN   33

struct TWiFiInfo{
  char           SSID[MAX_SSID_LEN];
  WIFIPROTECTION EncrMode;
  BYTE           SignalLevel;

  TWiFiInfo(const char* ssid = 0,
            WIFIPROTECTION encmode = WPT_NONE,
            BYTE level = 0
           )
  {
    _safecopy(SSID, ssid);
    EncrMode = encmode;
    SignalLevel = level;
  };
};

struct TWifiParams{
  TWiFiInfo      Info;
  char           Pwd [MAX_PWD_LEN];
  BYTE           Channel;

  TWifiParams(const TWiFiInfo& info,
              const char* pwd = 0, BYTE channel = 0)
  {
    Info = info;
    _safecopy(Pwd,  pwd);
    Channel  = channel;
  };
};

#define MAX_PPP_LOGIN 33
#define MAX_PPP_PWD   33

struct TPppParams{
  char Login[MAX_PPP_LOGIN];
  char Pwd  [MAX_PPP_PWD];

  TPppParams(const char* login = 0, const char* pwd = 0)
  {
    _safecopy(Login, login);
    _safecopy(Pwd,   pwd);
  };
};

#define MAX_PHONE_NUM  33
#define MAX_INIT_STR  129

struct TDialupParams{
  char    Phone  [MAX_PHONE_NUM];
  char    InitStr[MAX_INIT_STR];
  BOOLEAN bPulseMode;

  TDialupParams(const char* init_str = 0,
                const char* phone = 0,
                BOOLEAN pulse_mode = 0
               )
  {
    _safecopy(Phone,   phone);
    _safecopy(InitStr, init_str);
    bPulseMode = pulse_mode;
  };
};


typedef enum{
  PAR_NONE = 'N',
  PAR_EVEN = 'E',
  PAR_ODD  = 'O'
}PARITY;

typedef enum{
  STB_ONE          = 1,
  STB_TWO          = 2
}STOPBITS;

typedef enum{
  CST_UNDEFINED = 0,
  CST_CALLING = 1,
  CST_ONLINE = 2,
  CST_BROKEN = 3
}CONNSTATE;

class IConnection {
public:
  virtual ~IConnection(){};

/*
  Returns -1 if connection is broken, or number of bytes sent otherwise
*/
  virtual int   Send(const BYTE* dataOut, DWORD dataLen) = 0;

/*
  Returns -1 if connection is broken,
  0 if no data is currently available,
  or number of bytes received otherwise
*/
  virtual int   Receive(BYTE* dataIn, DWORD maxLen) = 0;
  virtual DWORD GetPeerAddress() = 0;

  virtual CONNSTATE GetState() = 0;
  virtual Error     GetLastError() = 0;
};

typedef enum{
  NWS_DOWN            = 0,
  NWS_ACCESSING_MEDIA = 1,
  NWS_WAITING_DHCP    = 2,
  NWS_UP              = 3,
  NWS_ERROR           = 4
} NWINTSTATE;

class INetworkInterface : public IDevice {
public:
  virtual NWINTSTATE GetCurrentState() = 0;

  virtual Error GetNetworkError() = 0;

  virtual Error SetSelfParams(const TNetworkClientParam& self_ip) = 0;
  virtual Error GetSelfParams(TNetworkClientParam& self_ip) = 0;   // useful when DHCP is used

  virtual Error Call  (const TNetworkHostParam& host, IConnection**) = 0;
  virtual Error Listen(WORD port, IConnection**) = 0;
  virtual Error Ping(DWORD dwAddress) = 0;

  virtual Error ResolveAddressUsingDNS(const char* pHostAddr, DWORD* pIpAddr) = 0;
};

class IEthernetInterface : public INetworkInterface{
public:
};

class ILoopBackInterface : public INetworkInterface{
public:
};


class IRadioInterface : public INetworkInterface{
public:
  virtual BYTE GetSignalLevelOutOf5() = 0; 
};


class IWiFiInterface : public IRadioInterface{
public:

  virtual BOOLEAN GetFirstAvailableWiFiNet(TWiFiInfo& info) = 0;
  virtual BOOLEAN GetNextAvailableWiFiNet (TWiFiInfo& info) = 0;

  virtual Error SetupWiFi(const TWifiParams& wifi) = 0;
};


class IGprsInterface : public IRadioInterface{
public:
  virtual Error SetupPppParams(const TPppParams& ppp) = 0;        // login, pwd
  virtual Error SetupDialupParams(const TDialupParams& dial) = 0; //  APN

  virtual const char* GetOperatorName() = 0; // must be stored internally in IGprsInterface descendant
};

#endif
