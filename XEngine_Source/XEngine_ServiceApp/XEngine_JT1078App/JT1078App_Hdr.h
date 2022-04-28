#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <errno.h>
#include <string>
#include <list>
#include <thread>
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#else
#include <arpa/inet.h>
#include <sys/wait.h>
#endif
using namespace std;
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_Core/NetXApi_Define.h>
#include <XEngine_Include/XEngine_Core/NetXApi_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Error.h>
#include "../../XEngine_UserProtocol.h"
#include "../../XEngine_ModuleConfigure/ModuleConfig_Define.h"
#include "../../XEngine_ModuleConfigure/ModuleConfig_Error.h"
#include "../../XEngine_ModuleDatabase/ModuleDB_Define.h"
#include "../../XEngine_ModuleDatabase/ModuleDB_Error.h"
#include "../../XEngine_ModuleHelp/ModuleHelp_Define.h"
#include "../../XEngine_ModuleHelp/ModuleHelp_Error.h"
#include "../../XEngine_ModuleSession/ModuleSession_Define.h"
#include "../../XEngine_ModuleSession/ModuleSession_Error.h"
#include "../../XEngine_ModuleProtocol/ModuleProtocol_Define.h"
#include "../../XEngine_ModuleProtocol/ModuleProtocol_Error.h"

#define XENGINE_STREAMAPP_RTPPACKET_BUFFER (1024 * 1024 * 2)

typedef enum
{
	ENUM_XENGINE_STREAMAPP_DEVICE_TYPE_RECORD = 0,
	ENUM_XENGINE_STREAMAPP_DEVICE_TYPE_STREAM = 1
}ENUM_XENGINE_STREAMAPP_DEVICE_TYPE;

extern BOOL bIsRun;
extern XLOG xhLog;

extern XNETHANDLE xhStreamNet;
extern XNETHANDLE xhStreamHeart;
extern XNETHANDLE xhStreamPool;
extern XHANDLE xhStreamPkt;

extern XNETHANDLE xhRecordNet;
extern XNETHANDLE xhRecordHeart;
extern XNETHANDLE xhRecordPool;
extern XHANDLE xhRecordPkt;

extern XENGINE_SERVICECONFIG st_ServiceCfg;
extern XENGINE_JT1078CONFIG st_JT1078Cfg;

#include "JT1078App_Config.h"
#include "JT1078App_Client.h"
#include "JT1078App_Stream.h"
#include "JT1078App_Record.h"
#include "JT1078App_Net.h"

#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Core/XEngine_Core")
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool")
#pragma comment(lib,"XEngine_Core/XEngine_NetXApi")
#pragma comment(lib,"XEngine_Client/XClient_Socket")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Packets")
#ifdef _DEBUG
#pragma comment(lib,"../../Debug/XEngine_ModuleConfigure")
#pragma comment(lib,"../../Debug/XEngine_ModuleDatabase")
#pragma comment(lib,"../../Debug/XEngine_ModuleHelp")
#pragma comment(lib,"../../Debug/XEngine_ModuleSession")
#pragma comment(lib,"../../Debug/XEngine_ModuleProtocol")
#else
#pragma comment(lib,"../../Release/XEngine_ModuleConfigure")
#pragma comment(lib,"../../Release/XEngine_ModuleDatabase")
#pragma comment(lib,"../../Release/XEngine_ModuleHelp")
#pragma comment(lib,"../../Release/XEngine_ModuleSession")
#pragma comment(lib,"../../Release/XEngine_ModuleProtocol")
#endif
#endif