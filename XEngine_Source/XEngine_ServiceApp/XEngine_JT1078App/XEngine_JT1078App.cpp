#include "JT1078App_Hdr.h"
/********************************************************************
//    Created:     2022/04/24  14:42:24
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App\XEngine_JT1078App.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App
//    File Base:   XEngine_JT1078App
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     主服务入口
//    History:
*********************************************************************/
BOOL bIsRun = FALSE;
XLOG xhLog = NULL;

XNETHANDLE xhStreamNet = 0;
XNETHANDLE xhStreamHeart = 0;
XNETHANDLE xhStreamPool = 0;
XHANDLE xhStreamPkt = NULL;

XNETHANDLE xhRecordNet = 0;
XNETHANDLE xhRecordHeart = 0;
XNETHANDLE xhRecordPool = 0;
XHANDLE xhRecordPkt = NULL;

XENGINE_SERVICECONFIG st_ServiceCfg;
XENGINE_JT1078CONFIG st_JT1078Cfg;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("流媒体流媒体服务器退出..."));
		bIsRun = FALSE;

		NetCore_TCPXCore_DestroyEx(xhStreamNet);
		NetCore_TCPXCore_DestroyEx(xhRecordNet);
		HelpComponents_PKTCustom_Destory(xhStreamPkt);
		HelpComponents_PKTCustom_Destory(xhRecordPkt);

		ManagePool_Thread_NQDestroy(xhStreamPool);
		ManagePool_Thread_NQDestroy(xhRecordPool);

		SocketOpt_HeartBeat_DestoryEx(xhStreamHeart);
		SocketOpt_HeartBeat_DestoryEx(xhRecordHeart);

		ModuleDB_JT1078_Destory();
		ModuleSession_JT1078Client_Destory();

		HelpComponents_XLog_Destroy(xhLog);
	}
	exit(0);
}

static int ServiceApp_Deamon(int wait)
{
#ifndef _MSC_BUILD
	pid_t pid = 0;
	int status;
	pid = fork();
	if (pid > 0)
	{
		exit(0);
	}

	close(2);
	while (1)
	{

		pid = fork();
		if (pid < 0)
			exit(1);
		if (pid == 0)
		{
			return 0;
		}
		waitpid(pid, &status, 0);

		if (wait > 0)
			sleep(wait);
	}
#endif
	return 0;
}

int main(int argc, char** argv)
{
	bIsRun = TRUE;
	int nCountStream = 0;
	int nCountRecord = 0;
	THREADPOOL_PARAMENT** ppSt_ListStream;
	THREADPOOL_PARAMENT** ppSt_ListRecord;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceCfg, '\0', sizeof(XENGINE_SERVICECONFIG));
	memset(&st_JT1078Cfg, '\0', sizeof(XENGINE_JT1078CONFIG));

	if (!JT1078App_Service_Parament(argc, argv))
	{
		return -1;
	}
	st_XLogConfig.XLog_MaxBackupFile = st_ServiceCfg.st_XLog.nMaxCount;
	st_XLogConfig.XLog_MaxSize = st_ServiceCfg.st_XLog.nMaxSize;
	_stprintf(st_XLogConfig.tszFileName, _T("./XEngine_XLog/XEngine_JT1078App.log"));

	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_FILE | HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("启动服务器失败，启动日志失败，错误：%X", XLog_GetLastError());
		goto XENGINE_EXITAPP;
	}
	HelpComponents_XLog_SetLogPriority(xhLog, st_ServiceCfg.st_XLog.nLogLeave);

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化日志系统成功"));
	if (st_ServiceCfg.bDeamon)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，使用守护进程启动服务..."));
		ServiceApp_Deamon(1);
	}
	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化服务器信号管理成功"));

	if (!ModuleDB_JT1078_Init((DATABASE_MYSQL_CONNECTINFO*)&st_ServiceCfg.st_XSql))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化JT1078数据库失败，错误：%lX"), ModuleDB_GetLastError());
		goto XENGINE_EXITAPP;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化JT1078数据库成功"));
	ModuleDB_JT1078_DeviceClear();
	//启动直播流
	if (st_JT1078Cfg.nStreamPort > 0)
	{
		xhStreamPkt = HelpComponents_PKTCustom_Init(st_JT1078Cfg.st_XMax.nMaxQueue, st_JT1078Cfg.st_XMax.nStreamThread);
		if (NULL == xhStreamPkt)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化实时端流包管理器失败，错误：%lX"), Packets_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化实时端流包管理器成功,最大队列:%d,最大线程:%d"), st_JT1078Cfg.st_XMax.nMaxQueue, st_JT1078Cfg.st_XMax.nStreamThread);
		HelpComponents_PKTCustom_SetHdrEx(xhStreamPkt, 24, 26, sizeof(XENGINE_RTPPACKETHDR2016));
		HelpComponents_PKTCustom_SetConditionsEx(xhStreamPkt, 15, 4, 4, -8, TRUE, TRUE);
		HelpComponents_PKTCustom_SetConditionsEx(xhStreamPkt, 15, 4, 0, 4, TRUE, TRUE);
		HelpComponents_PKTCustom_SetConditionsEx(xhStreamPkt, 15, 4, 1, 4, TRUE, TRUE);
		HelpComponents_PKTCustom_SetConditionsEx(xhStreamPkt, 15, 4, 2, 4, TRUE, TRUE);

		if (st_JT1078Cfg.st_XTime.nStreamTimeout > 0)
		{
			if (!SocketOpt_HeartBeat_InitEx(&xhStreamHeart, st_JT1078Cfg.st_XTime.nStreamTimeout, st_JT1078Cfg.st_XTime.nTimeCheck, XEngine_Callback_StreamHBLeave))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化实时端心跳管理服务失败，错误：%lX"), NetCore_GetLastError());
				goto XENGINE_EXITAPP;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化实时端心跳管理服务成功，句柄：%llu,检测时间:%d"), xhStreamHeart, st_JT1078Cfg.st_XTime.nStreamTimeout);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中，实时端心跳管理服务没有启用!"));
		}

		if (!NetCore_TCPXCore_StartEx(&xhStreamNet, st_JT1078Cfg.nStreamPort, st_JT1078Cfg.st_XMax.nMaxClient, st_JT1078Cfg.st_XMax.nIOThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动实时端网络服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动实时端网络服务成功，句柄：%llu，端口：%d"), xhStreamNet, st_JT1078Cfg.nStreamPort);
		NetCore_TCPXCore_RegisterCallBackEx(xhStreamNet, XEngine_Callback_StreamLogin, XEngine_Callback_StreamRecv, XEngine_Callback_StreamLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，注册实时端网络服务事件成功！"));

		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListStream, st_JT1078Cfg.st_XMax.nStreamThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_JT1078Cfg.st_XMax.nStreamThread; i++)
		{
			int* pInt_Pos = new int;

			*pInt_Pos = i;
			ppSt_ListStream[i]->lParam = pInt_Pos;
			ppSt_ListStream[i]->fpCall_ThreadsTask = XEngine_Stream_Thread;
		}
		if (!ManagePool_Thread_NQCreate(&xhStreamPool, &ppSt_ListStream, st_JT1078Cfg.st_XMax.nStreamThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动实时端处理线程池失败，错误：%d"), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动实时端处理线程池成功,线程个数:%d"), st_JT1078Cfg.st_XMax.nStreamThread);
	}
	//启动录像流
	if (st_JT1078Cfg.nRecordPort > 0)
	{
		xhRecordPkt = HelpComponents_PKTCustom_Init(st_JT1078Cfg.st_XTime.nRecordTimeout, st_JT1078Cfg.st_XMax.nRecordThread);
		if (NULL == xhRecordPkt)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化录像端流包管理器失败，错误：%lX"), Packets_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化录像端流包管理器成功,最大队列:%d,最大线程:%d"), st_JT1078Cfg.st_XMax.nMaxQueue, st_JT1078Cfg.st_XMax.nRecordThread);
		HelpComponents_PKTCustom_SetHdrEx(xhRecordPkt, 24, 26, sizeof(XENGINE_RTPPACKETHDR2016));
		HelpComponents_PKTCustom_SetConditionsEx(xhRecordPkt, 15, 4, 4, -8, TRUE, TRUE);
		HelpComponents_PKTCustom_SetConditionsEx(xhRecordPkt, 15, 4, 0, 4, TRUE, TRUE);
		HelpComponents_PKTCustom_SetConditionsEx(xhRecordPkt, 15, 4, 1, 4, TRUE, TRUE);
		HelpComponents_PKTCustom_SetConditionsEx(xhRecordPkt, 15, 4, 2, 4, TRUE, TRUE);

		if (st_JT1078Cfg.st_XTime.nRecordTimeout > 0)
		{
			if (!SocketOpt_HeartBeat_InitEx(&xhRecordHeart, st_JT1078Cfg.st_XTime.nRecordTimeout, st_JT1078Cfg.st_XTime.nTimeCheck, XEngine_Callback_RecordHBLeave))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，初始化录像端心跳管理服务失败，错误：%lX"), NetCore_GetLastError());
				goto XENGINE_EXITAPP;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化录像端心跳管理服务成功，句柄：%llu,检测时间:%d"), xhRecordHeart, st_JT1078Cfg.st_XTime.nRecordTimeout);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("启动服务中，录像端心跳管理服务没有启用!"));
		}

		if (!NetCore_TCPXCore_StartEx(&xhRecordNet, st_JT1078Cfg.nRecordPort, st_JT1078Cfg.st_XMax.nMaxClient, st_JT1078Cfg.st_XMax.nIOThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务器中，启动录像端网络服务失败，错误：%lX"), NetCore_GetLastError());
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动录像端网络服务成功，句柄：%llu，端口：%d"), xhRecordNet, st_JT1078Cfg.nRecordPort);
		NetCore_TCPXCore_RegisterCallBackEx(xhRecordNet, XEngine_Callback_RecordLogin, XEngine_Callback_RecordRecv, XEngine_Callback_RecordLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，注册录像端网络服务事件成功！"));

		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListRecord, st_JT1078Cfg.st_XMax.nRecordThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_JT1078Cfg.st_XMax.nRecordThread; i++)
		{
			int* pInt_Pos = new int;

			*pInt_Pos = i;
			ppSt_ListRecord[i]->lParam = pInt_Pos;
			ppSt_ListRecord[i]->fpCall_ThreadsTask = XEngine_Record_Thread;
		}
		if (!ManagePool_Thread_NQCreate(&xhRecordPool, &ppSt_ListRecord, st_JT1078Cfg.st_XMax.nRecordThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("启动服务中，启动录像端处理线程池失败，错误：%d"), errno);
			goto XENGINE_EXITAPP;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动录像端处理线程池成功,线程个数:%d"), st_JT1078Cfg.st_XMax.nStreamThread);
	}
	//标准协议服务
	for (int i = 0; i < st_JT1078Cfg.st_XClient.nMaxConnect; i++)
	{
		XNETHANDLE xhClient;
		XClient_TCPSelect_StartEx(&xhClient, st_JT1078Cfg.st_XClient.tszIPAddr, st_JT1078Cfg.st_XClient.nPort, 2, XEngine_Client_CBRecv, &xhClient, TRUE);
		XClient_TCPSelect_HBStartEx(xhClient);
		ModuleSession_JT1078Client_Create(xhClient);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，启动推流客户端成功,需要启动个数:%d,当前:%d"), st_JT1078Cfg.st_XClient.nMaxConnect, i);
	}

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("所有服务成功启动，JT1078流媒体服务运行中，XEngine版本:%s,发行次数:%d,当前运行版本：%s。。。"), XENGINE_VERSION_STR, st_JT1078Cfg.st_XVer.pStl_ListVer->size(), st_JT1078Cfg.st_XVer.pStl_ListVer->front().c_str());
	while (bIsRun)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

XENGINE_EXITAPP:
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("流媒体有服务关闭，流媒体服务器退出..."));

	if (bIsRun)
	{
		bIsRun = FALSE;

		NetCore_TCPXCore_DestroyEx(xhStreamNet);
		NetCore_TCPXCore_DestroyEx(xhRecordNet);

		HelpComponents_PKTCustom_Destory(xhStreamPkt);
		HelpComponents_PKTCustom_Destory(xhRecordPkt);

		ManagePool_Thread_NQDestroy(xhStreamPool);
		ManagePool_Thread_NQDestroy(xhRecordPool);

		SocketOpt_HeartBeat_DestoryEx(xhStreamHeart);
		SocketOpt_HeartBeat_DestoryEx(xhRecordHeart);

		ModuleDB_JT1078_Destory();
		ModuleSession_JT1078Client_Destory();

		HelpComponents_XLog_Destroy(xhLog);
	}

	return 0;
}