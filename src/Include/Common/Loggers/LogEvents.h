#ifndef _MGT_LOG_EVENTS_H
#define _MGT_LOG_EVENTS_H

#include "Common/Loggers/TLoggerToFile.h"
#include "Common/Utils/FormatConverters.h"

class TProgramStarted : public TNoDataPrintableEvent{
public: TProgramStarted() : TNoDataPrintableEvent(1, "Program started"){};
};

class TProgramStopped : public TNoDataPrintableEvent{
public: TProgramStopped() : TNoDataPrintableEvent(2, "Program stopped"){};
};

class TConnected : public TPrintableLogEvent{
public: TConnected(const std::string& peer) : TPrintableLogEvent(3, peer, LMT_ASCII, "Connected"){};
};

class TDisconnected : public TNoDataPrintableEvent{
public: TDisconnected() : TNoDataPrintableEvent(4, "Disconnected"){};
};

class TSent : public TPrintableLogEvent{
public: TSent(const T_BinaryData& data) : TPrintableLogEvent(5, data, LMT_DUMP, ">"){};
};

class TReceived : public TPrintableLogEvent{
public: TReceived(const T_BinaryData& data) : TPrintableLogEvent(6, data, LMT_DUMP, "<"){};
};

class TConnError : public TPrintableLogEvent{
public: TConnError(DWORD err_code) : TPrintableLogEvent(7, IntToStdString(err_code), LMT_ASCII, "Connection error"){};
};

class TAtrReceived : public TPrintableLogEvent{
public: TAtrReceived(const T_BinaryData& data) : TPrintableLogEvent(8, data, LMT_HEX, "ATR"){};
};

class TApduSent : public TPrintableLogEvent{
public: TApduSent(const T_BinaryData& data) : TPrintableLogEvent(9, data, LMT_DUMP, ">"){};
};

class TApduReceived : public TPrintableLogEvent{
public: TApduReceived(const T_BinaryData& data) : TPrintableLogEvent(10, data, LMT_DUMP, "<"){};
};

class TListeningOnPort : public TPrintableLogEvent{
public: TListeningOnPort(WORD port) : TPrintableLogEvent(11, IntToStdString(port), LMT_ASCII, "Listening on port"){};
};

class TNoTCPip : public TNoDataPrintableEvent{
public: TNoTCPip() : TNoDataPrintableEvent(12, "No TCP/IP interface available"){};
};

class TMgtLinkConnected : public TPrintableLogEvent{
public: TMgtLinkConnected(DWORD id) : TPrintableLogEvent(13, IntToStdString(id), LMT_ASCII, "MGT incoming link connected"){};
};

class TMgtLinkDisconnected : public TPrintableLogEvent{
public: TMgtLinkDisconnected(DWORD id) : TPrintableLogEvent(14, IntToStdString(id), LMT_ASCII, "MGT incoming link disconnected"){};
};

class TMgtTaskCreated : public TPrintableLogEvent{
public: TMgtTaskCreated(DWORD id, DWORD port_id, DWORD amt, DWORD op_type, const std::string& req_id) 
          : TPrintableLogEvent(15, std::string("TaskID=") + IntToStdString(id) + 
                                   ";PortID="+ IntToStdString(port_id) + 
                                   ";Amount="+ IntToStdString(amt) + 
                                   ";ReqID=" + req_id + 
                                   ";OpType=" + IntToStdString(op_type), 
                               LMT_ASCII, "MGT task created"){};
};

class TMgtTaskDestroyed : public TPrintableLogEvent{
public: TMgtTaskDestroyed(DWORD id, DWORD result_code)
          : TPrintableLogEvent(16, std::string("TaskID=") + IntToStdString(id) + ";Result=" + IntToStdString(result_code), 
                               LMT_ASCII, "MGT task destroyed"){};
};

#endif

