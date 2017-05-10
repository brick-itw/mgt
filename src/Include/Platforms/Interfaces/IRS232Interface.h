#ifndef I_RS232_INTERFACE
#define I_RS232_INTERFACE

#include "Platforms/Interfaces/IDevice.h"
#include "Platforms/Interfaces/IConnection.h"

struct TRs232Params{
  WORD      PortNumber;
  BYTE      DataBits;
  PARITY    Parity;
  STOPBITS  StopBits;
  DWORD     dwSpeed;

  TRs232Params(WORD portnum = 1,
               DWORD speed  = 115200,
               BYTE bits=8, PARITY par=PAR_NONE, STOPBITS stb=STB_ONE)
  {
    PortNumber = portnum;
    dwSpeed    = speed;
    DataBits   = bits;
    Parity     = par;
    StopBits   = stb;
  };
};

class IRS232Interface : public IDevice {
public:

  virtual Error Connect(const TRs232Params& par, IConnection** pConn) = 0;
};

#endif
