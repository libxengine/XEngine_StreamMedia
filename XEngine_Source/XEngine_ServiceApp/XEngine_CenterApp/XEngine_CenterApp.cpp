#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/01/20  14:33:32
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp\XEngine_CenterApp.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp
//    File Base:   XEngine_CenterApp
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     入口函数头文件
//    History:
*********************************************************************/
BOOL bIsRun = FALSE;
XLOG xhLog = NULL;
//业务服务器
XNETHANDLE xhCenterSocket = 0;
XNETHANDLE xhCenterHeart = 0;
XNETHANDLE xhCenterPool = 0;
XHANDLE xhCenterPacket = NULL;
//配置文件
XENGINE_SERVICECONFIG st_ServiceConfig;
XENGINE_JT1078CONFIG st_JT1078Config;
//调试用
FILE* pSt_FileVideo = NULL;
FILE* pSt_FileAudio = NULL;


void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("服务器退出..."));
		bIsRun = FALSE;
		//销毁业务资源
		NetCore_TCPXCore_DestroyEx(xhCenterSocket);
		SocketOpt_HeartBeat_DestoryEx(xhCenterHeart);
		HelpComponents_Datas_Destory(xhCenterPacket);
		ManagePool_Thread_NQDestroy(xhCenterPool);
		//销毁其他资源
		HelpComponents_XLog_Destroy(xhLog);
		if (NULL != pSt_FileAudio)
		{
			fclose(pSt_FileAudio);
		}
		if (NULL != pSt_FileVideo)
		{
			fclose(pSt_FileVideo);
		}
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
	bIsRun = TRUE;
	LPCTSTR lpszLogFile = _T("./XEngine_XLog/XEngine_CenterApp.Log");
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;
	THREADPOOL_PARAMENT** ppSt_ListCenterParam;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVICECONFIG));
	memset(&st_JT1078Config, '\0', sizeof(XENGINE_JT1078CONFIG));

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

	//调试相关
	pSt_FileVideo = _tfopen(_T("./Video.h264"), "wb");
	//启动业务服务相关代码
	if (st_ServiceConfig.nCenterPort > 0)
	{
		//组包器
		xhCenterPacket = HelpComponents_Datas_Init(st_ServiceConfig.st_XMax.nMaxQueue, st_ServiceConfig.st_XMax.nCenterThread);
		if (NULL == xhCenterPacket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,初始化业务组包器失败,错误：%lX"), Packets_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动业务组包器成功"));
		//启动心跳
		if (st_ServiceConfig.st_XTime.nCenterTimeOut > 0)
		{
			if (!SocketOpt_HeartBeat_InitEx(&xhCenterHeart, st_ServiceConfig.st_XTime.nCenterTimeOut, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_CenterHeart))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,初始化业务心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,初始化业务心跳服务成功,句柄:%llu,时间:%d,次数:%d"), xhCenterHeart, st_ServiceConfig.st_XTime.nCenterTimeOut, st_ServiceConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,业务心跳服务被设置为不启用"));
		}
		//启动网络
		if (!NetCore_TCPXCore_StartEx(&xhCenterSocket, st_ServiceConfig.nCenterPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启动业务网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动业务网络服务器成功,业务端口:%d,网络IO线程个数:%d"), st_ServiceConfig.nCenterPort, st_ServiceConfig.st_XMax.nIOThread);
		//绑定网络事件
		NetCore_TCPXCore_RegisterCallBackEx(xhCenterSocket, Network_Callback_CenterLogin, Network_Callback_CenterRecv, Network_Callback_CenterLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,注册业务网络事件成功"));
		//启动任务池
		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListCenterParam, st_ServiceConfig.st_XMax.nCenterThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ServiceConfig.st_XMax.nCenterThread; i++)
		{
			int* pInt_Pos = new int;

			*pInt_Pos = i;
			ppSt_ListCenterParam[i]->lParam = pInt_Pos;
			ppSt_ListCenterParam[i]->fpCall_ThreadsTask = XEngine_CenterTask_Thread;
		}
		if (!ManagePool_Thread_NQCreate(&xhCenterPool, &ppSt_ListCenterParam, st_ServiceConfig.st_XMax.nCenterThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中,启动业务线程池服务失败,错误：%lX"), ManagePool_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中,启动业务线程池服务成功,启动个数:%d"), st_ServiceConfig.st_XMax.nCenterThread);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中,业务消息服务没有被启用"));
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("所有服务成功启动,服务运行中,XEngine版本:%s,服务版本:%s,发行次数;%d。。。"), BaseLib_OperatorVer_XGetStr(), st_ServiceConfig.st_XVer.pStl_ListVer->front().c_str(), st_ServiceConfig.st_XVer.pStl_ListVer->size());
	while (bIsRun)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

XENGINE_SERVICEAPP_EXIT:
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("有服务启动失败,服务器退出..."));
		bIsRun = FALSE;
		//销毁业务资源
		NetCore_TCPXCore_DestroyEx(xhCenterSocket);
		SocketOpt_HeartBeat_DestoryEx(xhCenterHeart);
		HelpComponents_Datas_Destory(xhCenterPacket);
		ManagePool_Thread_NQDestroy(xhCenterPool);
		//销毁其他资源
		HelpComponents_XLog_Destroy(xhLog);
		if (NULL != pSt_FileAudio)
		{
			fclose(pSt_FileAudio);
		}
		if (NULL != pSt_FileVideo)
		{
			fclose(pSt_FileVideo);
		}
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}