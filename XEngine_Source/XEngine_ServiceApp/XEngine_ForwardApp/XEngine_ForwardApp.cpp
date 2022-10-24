#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/10/17  10:59:40
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp\XEngine_ForwardApp.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp
//    File Base:   XEngine_ForwardApp
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     入口函数头文件
//    History:
*********************************************************************/
BOOL bIsRun = FALSE;
XLOG xhLog = NULL;
//HTTP服务器
XHANDLE xhHttpSocket = NULL;
XHANDLE xhHttpHeart = NULL;
XHANDLE xhHttpPacket = NULL;
XHANDLE xhHttpPool = NULL;
//配置文件
XENGINE_SERVICECONFIG st_ServiceConfig;
XENGINE_FORWARDCONFIG st_ForwardConfig;


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

	SetDllDirectory(_T("./XEngine_Plugin"));
#endif
	bIsRun = TRUE;
	LPCTSTR lpszHTTPMime = _T("./XEngine_Config/HttpMime.types");
	LPCTSTR lpszHTTPCode = _T("./XEngine_Config/HttpCode.types");
	LPCTSTR lpszLogFile = _T("./XEngine_XLog/XEngine_ForwardApp.Log");
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;
	THREADPOOL_PARAMENT** ppSt_ListHTTPParam;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVICECONFIG));
	memset(&st_ForwardConfig, '\0', sizeof(XENGINE_FORWARDCONFIG));

	st_XLogConfig.XLog_MaxBackupFile = 10;
	st_XLogConfig.XLog_MaxSize = 1024000;
	_tcscpy(st_XLogConfig.tszFileName, lpszLogFile);
	//初始化参数
	if (!XEngine_Configure_Parament(argc, argv))
	{
		return -1;
	}
	//判断是否以守护进程启动
	if (st_ForwardConfig.bDeamon)
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
	if (st_ForwardConfig.nHttpPort > 0)
	{
		//HTTP包处理器
		xhHttpPacket = RfcComponents_HttpServer_InitEx(lpszHTTPCode, lpszHTTPMime, st_ForwardConfig.st_XMax.nHTTPThread);
		if (NULL == xhHttpPacket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,初始化HTTP组包失败,错误：%lX"), HttpServer_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化HTTP组包成功,IO线程个数:%d"), st_ForwardConfig.st_XMax.nHTTPThread);
		//启动心跳
		if (st_ForwardConfig.st_XTime.nHTTPTimeOut > 0)
		{
			xhHttpHeart = SocketOpt_HeartBeat_InitEx(st_ForwardConfig.st_XTime.nHTTPTimeOut, st_ForwardConfig.st_XTime.nTimeCheck, Network_Callback_HttpHeart);
			if (NULL == xhHttpHeart)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,初始化HTTP心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化HTTP心跳服务成功,句柄:%llu,时间:%d,次数:%d"), xhHttpHeart, st_ForwardConfig.st_XTime.nHTTPTimeOut, st_ForwardConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,HTTP心跳服务被设置为不启用"));
		}
		//网络
		xhHttpSocket = NetCore_TCPXCore_StartEx(st_ForwardConfig.nHttpPort, st_ForwardConfig.st_XMax.nMaxClient, st_ForwardConfig.st_XMax.nIOThread);
		if (NULL == xhHttpSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启动HTTP网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动HTTP网络服务器成功,HTTP端口:%d,IO:%d"), st_ForwardConfig.nHttpPort, st_ForwardConfig.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhHttpSocket, Network_Callback_HttpLogin, Network_Callback_HttpRecv, Network_Callback_HttpLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,注册HTTP网络事件成功"));
		//HTTP任务池
		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListHTTPParam, st_ForwardConfig.st_XMax.nHTTPThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ForwardConfig.st_XMax.nHTTPThread; i++)
		{
			int* pInt_Pos = new int;

			*pInt_Pos = i;
			ppSt_ListHTTPParam[i]->lParam = pInt_Pos;
			ppSt_ListHTTPParam[i]->fpCall_ThreadsTask = XEngine_HTTPTask_Thread;
		}
		xhHttpPool = ManagePool_Thread_NQCreate(&ppSt_ListHTTPParam, st_ForwardConfig.st_XMax.nHTTPThread);
		if (NULL == xhHttpPool)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启动HTTP线程池服务失败,错误：%lX"), ManagePool_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动HTTP线程池服务成功,启动个数:%d"), st_ForwardConfig.st_XMax.nHTTPThread);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,HTTP消息服务没有被启用"));
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("所有服务成功启动,服务运行中,XEngine版本:%s,服务版本:%s,发行次数;%d。。。"), BaseLib_OperatorVer_XGetStr(), st_ForwardConfig.st_XVer.pStl_ListVer->front().c_str(), st_ForwardConfig.st_XVer.pStl_ListVer->size());

	while (TRUE)
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
		HelpComponents_XLog_Destroy(xhLog);
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}