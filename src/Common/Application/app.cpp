#include "Common/Application/app.h"
#include "Platforms/Implementations/PC/Win/TIniFile.h"
#include "Common/Application/tsystem.h"
#include "Common/Loggers/LogEvents.h"
#include "Common/Utils/FormatConverters.h"
#include <winsock.h>

#include <string>

const std::string cIniFileName("mgt.ini");
const WORD cDefaultPort = 1130;

const std::string cWay4("Way4");
const std::string cListen("Listen");
const std::string cPort("Port");


TMgtApplication::TMgtApplication() : Way4Address(), bStopFlag(FALSE)
{
  TIniFile Params(cIniFileName);

  ListeningPort = (WORD)IntFromStdString(Params.GetString(cListen, cPort));
  if(!ListeningPort)
    ListeningPort = cDefaultPort;

  dwFirstTID = IntFromStdString(Params.GetString(cWay4, "FirstTID"));
  dwLastTID  = IntFromStdString(Params.GetString(cWay4, "LastTID"));
  if(dwFirstTID > dwLastTID)
    SwapDWORDS(&dwFirstTID, &dwLastTID);
  TidStatusArray = new BYTE[dwLastTID - dwFirstTID+1];

  Way4Address.dwAddress = inet_addr(Params.GetString(cWay4, "IPaddr").c_str());
  Way4Address.wPort = (WORD)IntFromStdString(Params.GetString(cWay4, cPort));
  Way4Address.UseSSL = (BOOLEAN)IntFromStdString(Params.GetString(cWay4, "UseSSL"));
  strcpy(Way4Address.SslDevFile, Params.GetString(cWay4, "DeviceSslFile").c_str());
  strcpy(Way4Address.SslHostCert, Params.GetString(cWay4, "HostSslCert").c_str());
}

TMgtApplication::~TMgtApplication()
{
  for(std::map<TMgtPortHandler*>::iterator it = ActivePorts.begin();  it != ActivePorts.end(); ++it)
    delete it->second;

  for(std::list<TMgtTask*>::iterator it = ActiveTasks.begin();  it != ActiveTasks.end(); ++it)
    delete *it;

  delete[] TidStatusArray;
}

void TMgtApplication::AddIncomingRequest(DWORD dwPortHandlerID, const T_Trivial_TLV_Buffer& Request)
{


  ActiveTasks.push_back(new TMgtTask(dwPortHandlerID, Request));
}


void TMgtApplication::HandleExpiredTask(TMgtTask* pTask)
{
  pTask
}

void TMgtApplication::RemoveTaskForBrokenLinkID(TMgtTask* pTask)
{
  if(!pTask->NeedsReversal())
    pTask->SetStatus(TS_DONE);
  else
  if(!pTask->ReversalInProgress())
    TaskProcessor->StartReversal(pTask);
}

void TMgtApplication::RemoveTasksForBrokenLinkID(DWORD dwID)
{
  for(std::list<TMgtTask*>::iterator it = ActiveTasks.begin();  it != ActiveTasks.end(); ++it)
    if(*it->GetPortHandlerID() == dwID)
      RemoveTaskForBrokenLink(*it);
}

void TMgtApplication::ProcessTasks()
{
  for(std::list<TMgtTask*>::iterator it = ActiveTasks.begin();  it != ActiveTasks.end();)
  {
    *it->Process();
    if(*it->Complete())
    {
      delete *it;
      it = ActiveTasks.erase(it);
    }
    else
    {
      if((*it->ReadyToDeliver())
        DeliverTaskResult(*it);
      else
      if(*it->Expired())
        HandleExpiredTask(*it);
      ++it;
    }
  }
}

void TMgtApplication::CheckRequestsOnActivePorts()
{
  T_Trivial_TLV_Buffer Request;

  for(std::map<TMgtPortHandler*>::iterator it = ActivePorts.begin();
      it != ActivePorts.end();
     )
  {
    if(it->second->IsLinkBroken())
    {
      RemoveTasksForBrokenLinkID(it->second->GetPortHandlerID());
      delete it->second;
      it = ActivePorts.erase(it);
    }
    else
    {
      if(it->second->GetIncomingRequest(Request))
        AddIncomingRequest(it->second->GetPortHandlerID(), Request);
      ++it;
    }
  }
}

void TMgtApplication::AddNewPortHandler(TMgtPortHandler* p)
{
  ActivePorts[p->GetPortHandlerID()] = p;
}

void TMgtApplication::Run()
{
  IEthernetInterface* pIface = (IEthernetInterface*)ISystem::Instance()->GetDeviceLayer()->GetDevice(DVC_ETHERNET, 0);
  if(!pIface)
  {
    TSystem::Instance().LogData(TNoTCPip());
    return;
  }

  IConnection* pNewLink = NULL; 
  pIface->Listen(ListeningPort, &pNewLink);

  for(;!bStopFlag;)
  {
    if(pNewLink && (CST_ONLINE == pNewLink->GetState()))
    {
      AddNewPortHandler(new TMgtPortHandler(pNewLink));
      pIface->Listen(ListeningPort, &pNewLink);
    }

    CheckRequestsOnActivePorts();
    ProcessTasks();
  }
}

