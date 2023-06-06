﻿#pragma once
//加载系统头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <thread>
#include <list>
#include <string>
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#endif
using namespace std;
//加载XEngine头文件
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/Packets_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/StreamClient_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/StreamClient_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/FLVProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/FLVProtocol_Error.h>
//加载项目相关头文件
#include "../../XEngine_UserProtocol.h"
#include "../../XEngine_ModuleConfigure/ModuleConfig_Define.h"
#include "../../XEngine_ModuleConfigure/ModuleConfig_Error.h"
#include "../../XEngine_ModuleSession/ModuleSession_Define.h"
#include "../../XEngine_ModuleSession/ModuleSession_Error.h"
#include "../../XEngine_ModuleProtocol/ModuleProtocol_Define.h"
#include "../../XEngine_ModuleProtocol/ModuleProtocol_Error.h"
#include "../../XEngine_ModuleHelp/ModuleHelp_Define.h"
#include "../../XEngine_ModuleHelp/ModuleHelp_Error.h"
//加载自己的头文件
#include "XEngine_Configure.h"
#include "XEngine_Network.h"
#include "XEngine_HttpTask.h"
#include "./StreamMedia_PushStream/PushStream_CenterTask.h"
#include "./StreamMedia_PushStream/PushStream_JT1078Task.h"
/********************************************************************
//    Created:     2023/06/04  10:35:46
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\XEngine_Hdr.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp
//    File Base:   XEngine_Hdr
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     公用头文件
//    History:
*********************************************************************/
extern bool bIsRun;
extern XHANDLE xhLog;
//HTTP服务器
extern XHANDLE xhHttpSocket;
extern XHANDLE xhHttpHeart;
extern XHANDLE xhHttpPacket;
extern XHANDLE xhHttpPool;
//XEngine协议推流服务
extern XHANDLE xhCenterSocket;
extern XHANDLE xhCenterHeart;
extern XHANDLE xhCenterPacket;
extern XHANDLE xhCenterPool;
//JT1078推流
extern XHANDLE xhJT1078Socket;
extern XHANDLE xhJT1078Heart;
extern XHANDLE xhJT1078Pkt;
extern XHANDLE xhJT1078Pool;
//配置文件
extern XENGINE_SERVICECONFIG st_ServiceConfig;

//连接库
#ifdef _MSC_BUILD
#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib,"../../x64/Debug/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../../x64/Debug/XEngine_ModuleProtocol.lib")
#pragma comment(lib,"../../x64/Debug/XEngine_ModuleSession.lib")
#pragma comment(lib,"../../x64/Debug/XEngine_ModuleHelp")
#else
#pragma comment(lib,"../../x64/Release/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../../x64/Release/XEngine_ModuleProtocol.lib")
#pragma comment(lib,"../../x64/Release/XEngine_ModuleSession.lib")
#pragma comment(lib,"../../x64/Release/XEngine_ModuleHelp")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib,"../../Debug/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../../Debug/XEngine_ModuleProtocol.lib")
#pragma comment(lib,"../../Debug/XEngine_ModuleSession.lib")
#pragma comment(lib,"../../Debug/XEngine_ModuleHelp")
#else
#pragma comment(lib,"../../Release/XEngine_ModuleConfigure.lib")
#pragma comment(lib,"../../Release/XEngine_ModuleProtocol.lib")
#pragma comment(lib,"../../Release/XEngine_ModuleSession.lib")
#pragma comment(lib,"../../Release/XEngine_ModuleHelp")
#endif
#endif
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_Packets")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_HttpProtocol.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_StreamClient.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_FLVProtocol.lib")
#pragma comment(lib,"Ws2_32.lib")
#endif