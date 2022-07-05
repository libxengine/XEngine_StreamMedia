#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/06/22  10:04:03
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_SDKApp\XEngine_SDKApp.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_SDKApp
//    File Base:   XEngine_SDKApp
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     入口函数头文件
//    History:
*********************************************************************/
BOOL bIsRun = FALSE;
XLOG xhLog = NULL;
//HTTP服务器
XNETHANDLE xhHttpSocket = 0;
XNETHANDLE xhHttpHeart = 0;
XNETHANDLE xhHttpPool = 0;
XHANDLE xhHttpPacket = NULL;
//配置文件
XENGINE_SERVICECONFIG st_ServiceConfig;
XENGINE_SDKCONFIG st_SDKConfig;;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("服务器退出..."));
		bIsRun = FALSE;
		//销毁HTTP资源
		NetCore_TCPXCore_DestroyEx(xhHttpSocket);
		SocketOpt_HeartBeat_DestoryEx(xhHttpHeart);
		RfcComponents_HttpServer_DestroyEx(xhHttpPacket);
		ManagePool_Thread_NQDestroy(xhHttpPool);
		//销毁其他资源
		ModuleSession_SDKDevice_Destory();
		ModulePlugin_Core_Destory();
		HelpComponents_XLog_Destroy(xhLog);
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	exit(0);
}
//LINUX守护进程
static int ServiceApp_Deamon()
{
#ifndef _MSC_BUILD
	pid_t nPID = 0;
	int nStatus = 0;
	nPID = fork();
	if (nPID > 0)
	{
		exit(0);
	}

	close(2);
	while (1)
	{
		nPID = fork();
		if (nPID < 0)
		{
			exit(1);
		}
		if (nPID == 0)
		{
			return 0;
		}
		waitpid(nPID, &nStatus, 0);
		sleep(1);
	}
#endif
	return 0;
}

int main(int argc, char** argv)
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	SetDllDirectory(_T("./XEngine_Plugin"));
	bIsRun = TRUE;
	LPCTSTR lpszHTTPMime = _T("./XEngine_Config/HttpMime.types");
	LPCTSTR lpszHTTPCode = _T("./XEngine_Config/HttpCode.types");
	LPCTSTR lpszLogFile = _T("./XEngine_XLog/XEngine_SDKApp.Log");
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;
	THREADPOOL_PARAMENT** ppSt_ListHTTPParam;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_SDKConfig, '\0', sizeof(XENGINE_SDKCONFIG));
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVICECONFIG));

	st_XLogConfig.XLog_MaxBackupFile = 10;
	st_XLogConfig.XLog_MaxSize = 1024000;
	_tcscpy(st_XLogConfig.tszFileName, lpszLogFile);
	//初始化参数
	if (!XEngine_Configure_Parament(argc, argv))
	{
		return -1;
	}
	//判断是否以守护进程启动
	if (st_ServiceConfig.bDeamon)
	{
		ServiceApp_Deamon();
	}
	//初始日志
	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_STD | HELPCOMPONENTS_XLOG_OUTTYPE_FILE, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("启动服务中,启动日志失败,错误：%lX", XLog_GetLastError());
		goto XENGINE_SERVICEAPP_EXIT;
	}
	//设置日志打印级别
	HelpComponents_XLog_SetLogPriority(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化日志系统成功"));

	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化信号量成功"));
	//启动HTTP服务相关代码
	if (st_SDKConfig.nHttpPort > 0)
	{
		//HTTP包处理器
		xhHttpPacket = RfcComponents_HttpServer_InitEx(lpszHTTPCode, lpszHTTPMime, st_SDKConfig.st_XMax.nHTTPThread);
		if (NULL == xhHttpPacket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,初始化HTTP组包失败,错误：%lX"), HttpServer_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化HTTP组包成功,IO线程个数:%d"), st_SDKConfig.st_XMax.nHTTPThread);
		//启动心跳
		if (st_SDKConfig.st_XTime.nHTTPTimeOut > 0)
		{
			if (!SocketOpt_HeartBeat_InitEx(&xhHttpHeart, st_SDKConfig.st_XTime.nHTTPTimeOut, st_SDKConfig.st_XTime.nTimeCheck, Network_Callback_HttpHeart))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,初始化HTTP心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化HTTP心跳服务成功,句柄:%llu,时间:%d,次数:%d"), xhHttpHeart, st_SDKConfig.st_XTime.nHTTPTimeOut, st_SDKConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,HTTP心跳服务被设置为不启用"));
		}
		//网络
		if (!NetCore_TCPXCore_StartEx(&xhHttpSocket, st_SDKConfig.nHttpPort, st_SDKConfig.st_XMax.nMaxClient, st_SDKConfig.st_XMax.nIOThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启动HTTP网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动HTTP网络服务器成功,HTTP端口:%d,IO:%d"), st_SDKConfig.nHttpPort, st_SDKConfig.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhHttpSocket, Network_Callback_HttpLogin, Network_Callback_HttpRecv, Network_Callback_HttpLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,注册HTTP网络事件成功"));
		//HTTP任务池
		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListHTTPParam, st_SDKConfig.st_XMax.nHTTPThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_SDKConfig.st_XMax.nHTTPThread; i++)
		{
			int* pInt_Pos = new int;

			*pInt_Pos = i;
			ppSt_ListHTTPParam[i]->lParam = pInt_Pos;
			ppSt_ListHTTPParam[i]->fpCall_ThreadsTask = XEngine_HTTPTask_Thread;
		}
		if (!ManagePool_Thread_NQCreate(&xhHttpPool, &ppSt_ListHTTPParam, st_SDKConfig.st_XMax.nHTTPThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启动HTTP线程池服务失败,错误：%lX"), ManagePool_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动HTTP线程池服务成功,启动个数:%d"), st_SDKConfig.st_XMax.nHTTPThread);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,HTTP消息服务没有被启用"));
	}
	//加载插件
	for (auto stl_ListIterator = st_SDKConfig.st_XPlugin.pStl_ListPlugin->begin(); stl_ListIterator != st_SDKConfig.st_XPlugin.pStl_ListPlugin->end(); stl_ListIterator++)
	{
		if (stl_ListIterator->bEnable)
		{
			if (ModulePlugin_Core_Insert(&stl_ListIterator->xhToken, stl_ListIterator->tszPluginFile))
			{
				if (ModulePlugin_Core_Init(stl_ListIterator->xhToken, stl_ListIterator->tszPluginAddr, stl_ListIterator->nPort, stl_ListIterator->tszPluginUser, stl_ListIterator->tszPluginPass, st_SDKConfig.st_XClient.nMaxClient))
				{
					//标准协议服务
					for (int i = 0; i < st_SDKConfig.st_XClient.nMaxClient; i++)
					{
						XNETHANDLE xhClient = 0;
						XClient_TCPSelect_StartEx(&xhClient, st_SDKConfig.st_XClient.tszIPAddr, st_SDKConfig.st_XClient.nPort, 2, XEngine_Client_CBRecv, NULL, TRUE);
						XClient_TCPSelect_HBStartEx(xhClient);
						ModuleSession_SDKDevice_Create(stl_ListIterator->xhToken);
						ModuleSession_SDKDevice_InsertClient(stl_ListIterator->xhToken, xhClient);
						XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动推流客户端成功,需要启动个数:%d,当前:%d,连接地址:%s,端口:%d"), st_SDKConfig.st_XClient.nMaxClient, i, st_SDKConfig.st_XClient.tszIPAddr, st_SDKConfig.st_XClient.nPort);
						//线程
						std::thread m_STDThread(XEngine_PluginTask_Thread, stl_ListIterator->xhToken, i);
						m_STDThread.detach();
					}
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,加载插件:%s,句柄:%lld 路径:%s,连接地址:%s,端口:%d 成功"), stl_ListIterator->tszPluginMethod, stl_ListIterator->xhToken, stl_ListIterator->tszPluginFile, st_SDKConfig.st_XClient.tszIPAddr, st_SDKConfig.st_XClient.nPort);
				}
				else
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,加载插件:%s 路径:%s 失败,初始化失败,错误码:%lX"), stl_ListIterator->tszPluginMethod, stl_ListIterator->tszPluginFile, ModulePlugin_GetLastError());
				}
			}
			else
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,加载插件:%s 路径:%s 失败,错误码:%lX"), stl_ListIterator->tszPluginMethod, stl_ListIterator->tszPluginFile, ModulePlugin_GetLastError());
			}
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,插件模块:%s 被设置为禁用"), stl_ListIterator->tszPluginFile);
		}
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,插件加载完毕,一共加载:%d 个插件"), st_SDKConfig.st_XPlugin.pStl_ListPlugin->size());
	
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("所有服务成功启动,服务运行中,XEngine版本:%s,服务版本:%s,发行次数;%d。。。"), BaseLib_OperatorVer_XGetStr(), st_SDKConfig.st_XVer.pStl_ListVer->front().c_str(), st_SDKConfig.st_XVer.pStl_ListVer->size());
	while (bIsRun)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

XENGINE_SERVICEAPP_EXIT:
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("有服务启动失败,服务器退出..."));
		bIsRun = FALSE;
		//销毁HTTP资源
		NetCore_TCPXCore_DestroyEx(xhHttpSocket);
		SocketOpt_HeartBeat_DestoryEx(xhHttpHeart);
		RfcComponents_HttpServer_DestroyEx(xhHttpPacket);
		ManagePool_Thread_NQDestroy(xhHttpPool);
		//销毁其他资源
		ModuleSession_SDKDevice_Destory();
		ModulePlugin_Core_Destory();
		HelpComponents_XLog_Destroy(xhLog);
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}