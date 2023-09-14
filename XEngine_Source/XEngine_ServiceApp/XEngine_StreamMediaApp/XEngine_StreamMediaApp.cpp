#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/06/04  10:37:32
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\XEngine_StreamMediaApp.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp
//    File Base:   XEngine_StreamMediaApp
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     入口函数头文件
//    History:
*********************************************************************/
bool bIsRun = false;
XHANDLE xhLog = NULL;
//HTTP服务器
XHANDLE xhHttpSocket = NULL;
XHANDLE xhHttpHeart = NULL;
XHANDLE xhHttpPacket = NULL;
XHANDLE xhHttpPool = NULL;
//XEngine推流服务
XHANDLE xhXStreamSocket = NULL;
XHANDLE xhXStreamHeart = NULL;
XHANDLE xhXStreamPacket = NULL;
XHANDLE xhXStreamPool = NULL;
//RTMP推流
XHANDLE xhRTMPSocket = NULL;
XHANDLE xhRTMPHeart = NULL;
XHANDLE xhRTMPPool = NULL;
XHANDLE xhSRTPool = NULL;
//JT1078推流
XHANDLE xhJT1078Socket = NULL;
XHANDLE xhJT1078Heart = NULL;
XHANDLE xhJT1078Pkt = NULL;
XHANDLE xhJT1078Pool = NULL;
//配置文件
XENGINE_SERVICECONFIG st_ServiceConfig;
//调试
FILE* pSt_VFile = NULL;
FILE* pSt_AFile = NULL;

void ServiceApp_Stop(int signo)
{
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("服务器退出..."));
		bIsRun = false;
		//销毁网络
		NetCore_TCPXCore_DestroyEx(xhHttpSocket);
		NetCore_TCPXCore_DestroyEx(xhXStreamSocket);
		NetCore_TCPXCore_DestroyEx(xhRTMPSocket);
		NetCore_TCPXCore_DestroyEx(xhJT1078Socket);
		//销毁心跳
		SocketOpt_HeartBeat_DestoryEx(xhHttpHeart);
		SocketOpt_HeartBeat_DestoryEx(xhXStreamHeart);
		SocketOpt_HeartBeat_DestoryEx(xhRTMPHeart);
		SocketOpt_HeartBeat_DestoryEx(xhJT1078Heart);
		//销毁包管理器
		HttpProtocol_Server_DestroyEx(xhHttpPacket);
		HelpComponents_Datas_Destory(xhXStreamPacket);
		HelpComponents_PKTCustom_Destory(xhJT1078Pkt);
		RTMPProtocol_Parse_Destory();
		FLVProtocol_Parse_Destory();
		HLSProtocol_TSParse_Destory();
		//销毁线程池
		ManagePool_Thread_NQDestroy(xhHttpPool);
		ManagePool_Thread_NQDestroy(xhXStreamPool);
		ManagePool_Thread_NQDestroy(xhRTMPPool);
		ManagePool_Thread_NQDestroy(xhJT1078Pool);
		//销毁其他资源
		ModuleHelp_SrtCore_Destory();
		srt_cleanup();

		HelpComponents_XLog_Destroy(xhLog);
		if (NULL != pSt_AFile)
		{
			fclose(pSt_AFile);
		}
		if (NULL != pSt_VFile)
		{
			fclose(pSt_VFile);
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
	srt_startup();
	srt_setloglevel(srt_logging::LogLevel::fatal);
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	bIsRun = true;
	LPCXSTR lpszHTTPMime = _X("./XEngine_Config/HttpMime.types");
	LPCXSTR lpszHTTPCode = _X("./XEngine_Config/HttpCode.types");
	LPCXSTR lpszLogFile = _X("./XEngine_XLog/XEngine_StreamMediaApp.Log");
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;
	THREADPOOL_PARAMENT** ppSt_ListHTTPParam;
	THREADPOOL_PARAMENT** ppSt_ListCenterParam;
	THREADPOOL_PARAMENT** ppSt_ListRTMPParam;
	THREADPOOL_PARAMENT** ppSt_ListJT1078Param;
	THREADPOOL_PARAMENT** ppSt_ListSRTParam;

	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceConfig, '\0', sizeof(XENGINE_SERVICECONFIG));

	pSt_VFile = _xtfopen("./1.ts", "wb");
	pSt_AFile = _xtfopen("./1.h264", "wb");

	st_XLogConfig.XLog_MaxBackupFile = 10;
	st_XLogConfig.XLog_MaxSize = 1024000;
	_tcsxcpy(st_XLogConfig.tszFileName, lpszLogFile);
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
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化日志系统成功"));

	signal(SIGINT, ServiceApp_Stop);
	signal(SIGTERM, ServiceApp_Stop);
	signal(SIGABRT, ServiceApp_Stop);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化信号量成功"));
	//启动HTTP服务相关代码
	if (st_ServiceConfig.nHttpPort > 0)
	{
		//HTTP包处理器
		xhHttpPacket = HttpProtocol_Server_InitEx(lpszHTTPCode, lpszHTTPMime, st_ServiceConfig.st_XMax.nHTTPThread);
		if (NULL == xhHttpPacket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化HTTP组包失败,错误：%lX"), HttpProtocol_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化HTTP组包成功,IO线程个数:%d"), st_ServiceConfig.st_XMax.nHTTPThread);
		//启动心跳
		if (st_ServiceConfig.st_XTime.nHTTPTimeout > 0)
		{
			xhHttpHeart = SocketOpt_HeartBeat_InitEx(st_ServiceConfig.st_XTime.nHTTPTimeout, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_HttpHeart);
			if (NULL == xhHttpHeart)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化HTTP心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化HTTP心跳服务成功,时间:%d,次数:%d"), st_ServiceConfig.st_XTime.nHTTPTimeout, st_ServiceConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,HTTP心跳服务被设置为不启用"));
		}
		//网络
		xhHttpSocket = NetCore_TCPXCore_StartEx(st_ServiceConfig.nHttpPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread);
		if (NULL == xhHttpSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,启动HTTP网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动HTTP网络服务器成功,HTTP端口:%d,IO:%d"), st_ServiceConfig.nHttpPort, st_ServiceConfig.st_XMax.nIOThread);
		NetCore_TCPXCore_RegisterCallBackEx(xhHttpSocket, Network_Callback_HttpLogin, Network_Callback_HttpRecv, Network_Callback_HttpLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,注册HTTP网络事件成功"));
		//HTTP任务池
		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListHTTPParam, st_ServiceConfig.st_XMax.nHTTPThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ServiceConfig.st_XMax.nHTTPThread; i++)
		{
			int* pInt_Pos = new int;

			*pInt_Pos = i;
			ppSt_ListHTTPParam[i]->lParam = pInt_Pos;
			ppSt_ListHTTPParam[i]->fpCall_ThreadsTask = XEngine_HTTPTask_Thread;
		}
		xhHttpPool = ManagePool_Thread_NQCreate(&ppSt_ListHTTPParam, st_ServiceConfig.st_XMax.nHTTPThread);
		if (NULL == xhHttpPool)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,启动HTTP线程池服务失败,错误：%lX"), ManagePool_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动HTTP线程池服务成功,启动个数:%d"), st_ServiceConfig.st_XMax.nHTTPThread);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,HTTP消息服务没有被启用"));
	}
	//启动标准推流代码
	if (st_ServiceConfig.nXStreamPort > 0)
	{
		//组包器
		xhXStreamPacket = HelpComponents_Datas_Init(st_ServiceConfig.st_XMax.nMaxQueue, st_ServiceConfig.st_XMax.nXStreamThread);
		if (NULL == xhXStreamPacket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化XEngine推流组包器失败,错误：%lX"), Packets_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动XEngine推流组包器成功"));
		//启动心跳
		if (st_ServiceConfig.st_XTime.nXStreamTimeout > 0)
		{
			xhXStreamHeart = SocketOpt_HeartBeat_InitEx(st_ServiceConfig.st_XTime.nXStreamTimeout, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_XStreamHeart);
			if (NULL == xhXStreamHeart)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化XEngine推流心跳服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化XEngine推流心跳服务成功,时间:%d,次数:%d"), st_ServiceConfig.st_XTime.nXStreamTimeout, st_ServiceConfig.st_XTime.nTimeCheck);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,XEngine推流心跳服务被设置为不启用"));
		}
		//启动网络
		xhXStreamSocket = NetCore_TCPXCore_StartEx(st_ServiceConfig.nXStreamPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread);
		if (NULL == xhXStreamSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,启动XEngine推流网络服务器失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动XEngine推流网络服务器成功,XEngine推流端口:%d,网络IO线程个数:%d"), st_ServiceConfig.nXStreamPort, st_ServiceConfig.st_XMax.nIOThread);
		//绑定网络事件
		NetCore_TCPXCore_RegisterCallBackEx(xhXStreamSocket, Network_Callback_XStreamLogin, Network_Callback_XStreamRecv, Network_Callback_XStreamLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,注册XEngine推流网络事件成功"));
		//启动任务池
		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListCenterParam, st_ServiceConfig.st_XMax.nXStreamThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ServiceConfig.st_XMax.nXStreamThread; i++)
		{
			int* pInt_Pos = new int;

			*pInt_Pos = i;
			ppSt_ListCenterParam[i]->lParam = pInt_Pos;
			ppSt_ListCenterParam[i]->fpCall_ThreadsTask = PushStream_XStreamTask_Thread;
		}
		xhXStreamPool = ManagePool_Thread_NQCreate(&ppSt_ListCenterParam, st_ServiceConfig.st_XMax.nXStreamThread);
		if (NULL == xhXStreamPool)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,启动XEngine推流线程池服务失败,错误：%lX"), ManagePool_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动XEngine推流线程池服务成功,启动个数:%d"), st_ServiceConfig.st_XMax.nXStreamThread);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,XEngine推流消息服务没有被启用"));
	}
	//启动RTMP流支持
	if (st_ServiceConfig.nRTMPPort > 0)
	{
		if (!RTMPProtocol_Parse_Init(st_ServiceConfig.st_XMax.nRTMPThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化RTMP端流包管理器失败,错误：%lX"), RTMPProtocol_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化RTMP端流包管理器成功,最大队列:%d,最大线程:%d"), st_ServiceConfig.st_XMax.nMaxQueue, st_ServiceConfig.st_XMax.nRTMPThread);
		
		if (st_ServiceConfig.st_XTime.nRTMPTimeout > 0)
		{
			xhRTMPHeart = SocketOpt_HeartBeat_InitEx(st_ServiceConfig.st_XTime.nRTMPTimeout, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_RTMPHeart);
			if (NULL == xhRTMPHeart)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化RTMP端心跳管理服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化RTMP端心跳管理服务成功,检测时间:%d"), st_ServiceConfig.st_XTime.nRTMPTimeout);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,实时RTMP端管理服务没有启用!"));
		}
		xhRTMPSocket = NetCore_TCPXCore_StartEx(st_ServiceConfig.nRTMPPort, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread);
		if (NULL == xhRTMPSocket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务器中,启动RTMP网络服务失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动RTMP端网络服务成功,端口：%d"), st_ServiceConfig.nRTMPPort);
		NetCore_TCPXCore_RegisterCallBackEx(xhRTMPSocket, Network_Callback_RTMPLogin, Network_Callback_RTMPRecv, Network_Callback_RTMPLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,注册RTMP端网络服务事件成功！"));

		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListRTMPParam, st_ServiceConfig.st_XMax.nRTMPThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ServiceConfig.st_XMax.nRTMPThread; i++)
		{
			int* pInt_Pos = new int;

			*pInt_Pos = i;
			ppSt_ListRTMPParam[i]->lParam = pInt_Pos;
			ppSt_ListRTMPParam[i]->fpCall_ThreadsTask = PushStream_RTMPTask_Thread;
		}
		xhRTMPPool = ManagePool_Thread_NQCreate(&ppSt_ListRTMPParam, st_ServiceConfig.st_XMax.nRTMPThread);
		if (NULL == xhRTMPPool)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,启动RTMP端处理线程池失败,错误：%d"), errno);
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动RTMP端处理线程池成功,线程个数:%d"), st_ServiceConfig.st_XMax.nRTMPThread);
	}
	//启动JT1078流支持
	if (st_ServiceConfig.nJT1078Port > 0)
	{
		xhJT1078Pkt = HelpComponents_PKTCustom_Init(st_ServiceConfig.st_XMax.nMaxQueue, st_ServiceConfig.st_XMax.nJT1078Thread);
		if (NULL == xhJT1078Pkt)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化实时端流包管理器失败,错误：%lX"), Packets_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化实时端流包管理器成功,最大队列:%d,最大线程:%d"), st_ServiceConfig.st_XMax.nMaxQueue, st_ServiceConfig.st_XMax.nJT1078Thread);
		//协议头大小.需要加上长度字段
		HelpComponents_PKTCustom_SetHdrEx(xhJT1078Pkt, 24, 26, sizeof(XENGINE_RTPPACKETHDR2016) + sizeof(XSHOT));
		//如果packet == 4,透传,没有时间戳
		HelpComponents_PKTCustom_SetConditionsEx(xhJT1078Pkt, 15, 4, 4, -8, true, true);
		//如果packet == 0,1,2 I帧P帧B帧,需要添加视频帧间隔时间,两个XSHOT大小
		HelpComponents_PKTCustom_SetConditionsEx(xhJT1078Pkt, 15, 4, 0, 4, true, true);
		HelpComponents_PKTCustom_SetConditionsEx(xhJT1078Pkt, 15, 4, 1, 4, true, true);
		HelpComponents_PKTCustom_SetConditionsEx(xhJT1078Pkt, 15, 4, 2, 4, true, true);

		if (st_ServiceConfig.st_XTime.nJT1078Timeout > 0)
		{
			xhJT1078Heart = SocketOpt_HeartBeat_InitEx(st_ServiceConfig.st_XTime.nJT1078Timeout, st_ServiceConfig.st_XTime.nTimeCheck, Network_Callback_JT1078HBLeave);
			if (NULL == xhJT1078Heart)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化实时端心跳管理服务失败,错误：%lX"), NetCore_GetLastError());
				goto XENGINE_SERVICEAPP_EXIT;
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化实时端心跳管理服务成功,检测时间:%d"), st_ServiceConfig.st_XTime.nJT1078Timeout);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("启动服务中,实时端心跳管理服务没有启用!"));
		}
		xhJT1078Socket = NetCore_TCPXCore_StartEx(st_ServiceConfig.nJT1078Port, st_ServiceConfig.st_XMax.nMaxClient, st_ServiceConfig.st_XMax.nIOThread);
		if (NULL == xhJT1078Socket)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务器中,启动实时端网络服务失败,错误：%lX"), NetCore_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动实时端网络服务成功,端口：%d"), st_ServiceConfig.nJT1078Port);
		NetCore_TCPXCore_RegisterCallBackEx(xhJT1078Socket, Network_Callback_JT1078Login, Network_Callback_JT1078Recv, Network_Callback_JT1078Leave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,注册实时端网络服务事件成功！"));

		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListJT1078Param, st_ServiceConfig.st_XMax.nJT1078Thread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ServiceConfig.st_XMax.nJT1078Thread; i++)
		{
			int* pInt_Pos = new int;

			*pInt_Pos = i;
			ppSt_ListJT1078Param[i]->lParam = pInt_Pos;
			ppSt_ListJT1078Param[i]->fpCall_ThreadsTask = PushStream_JT1078Task_Thread;
		}
		xhJT1078Pool = ManagePool_Thread_NQCreate(&ppSt_ListJT1078Param, st_ServiceConfig.st_XMax.nJT1078Thread);
		if (NULL == xhJT1078Pool)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,启动实时端处理线程池失败,错误：%d"), errno);
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动实时端处理线程池成功,线程个数:%d"), st_ServiceConfig.st_XMax.nJT1078Thread);
	}
	if (st_ServiceConfig.nSrtPort > 0)
	{
		if (!ModuleHelp_SrtCore_Start(st_ServiceConfig.nSrtPort))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,启动SRT服务失败,错误：%s"), srt_getlasterror_str());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		ModuleHelp_SrtCore_SetCallback(Network_Callback_SRTLogin, Network_Callback_SRTRecv, Network_Callback_SRTLeave);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动SRT服务成功,绑定的端口:%d"), st_ServiceConfig.nSrtPort);

		if (!HLSProtocol_TSParse_Init(st_ServiceConfig.st_XMax.nSRTThread))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,初始化RTMP端流包管理器失败,错误：%lX"), RTMPProtocol_GetLastError());
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,初始化SRT端流包管理器成功,最大线程:%d"), st_ServiceConfig.st_XMax.nSRTThread);

		BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ListSRTParam, st_ServiceConfig.st_XMax.nSRTThread, sizeof(THREADPOOL_PARAMENT));
		for (int i = 0; i < st_ServiceConfig.st_XMax.nSRTThread; i++)
		{
			int* pInt_Pos = new int;

			*pInt_Pos = i;
			ppSt_ListSRTParam[i]->lParam = pInt_Pos;
			ppSt_ListSRTParam[i]->fpCall_ThreadsTask = PushStream_SRTTask_Thread;
		}
		xhSRTPool = ManagePool_Thread_NQCreate(&ppSt_ListSRTParam, st_ServiceConfig.st_XMax.nSRTThread);
		if (NULL == xhSRTPool)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("启动服务中,启动SRT端处理线程池失败,错误：%d"), errno);
			goto XENGINE_SERVICEAPP_EXIT;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("启动服务中,启动SRT端处理线程池成功,线程个数:%d"), st_ServiceConfig.st_XMax.nSRTThread);
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("所有服务成功启动,服务运行中,XEngine版本:%s,服务版本:%s,发行次数;%d。。。"), BaseLib_OperatorVer_XNumberStr(), st_ServiceConfig.st_XVer.pStl_ListVer->front().c_str(), st_ServiceConfig.st_XVer.pStl_ListVer->size());

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

XENGINE_SERVICEAPP_EXIT:
	if (bIsRun)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("有服务启动失败,服务器退出..."));
		bIsRun = false;
		//销毁网络
		NetCore_TCPXCore_DestroyEx(xhHttpSocket);
		NetCore_TCPXCore_DestroyEx(xhXStreamSocket);
		NetCore_TCPXCore_DestroyEx(xhRTMPSocket);
		NetCore_TCPXCore_DestroyEx(xhJT1078Socket);
		//销毁心跳
		SocketOpt_HeartBeat_DestoryEx(xhHttpHeart);
		SocketOpt_HeartBeat_DestoryEx(xhXStreamHeart);
		SocketOpt_HeartBeat_DestoryEx(xhRTMPHeart);
		SocketOpt_HeartBeat_DestoryEx(xhJT1078Heart);
		//销毁包管理器
		HttpProtocol_Server_DestroyEx(xhHttpPacket);
		HelpComponents_Datas_Destory(xhXStreamPacket);
		HelpComponents_PKTCustom_Destory(xhJT1078Pkt);
		RTMPProtocol_Parse_Destory();
		FLVProtocol_Parse_Destory();
		HLSProtocol_TSParse_Destory();
		//销毁线程池
		ManagePool_Thread_NQDestroy(xhHttpPool);
		ManagePool_Thread_NQDestroy(xhXStreamPool);
		ManagePool_Thread_NQDestroy(xhRTMPPool);
		ManagePool_Thread_NQDestroy(xhJT1078Pool);
		//销毁其他资源
		ModuleHelp_SrtCore_Destory();
		srt_cleanup();

		HelpComponents_XLog_Destroy(xhLog);
		if (NULL != pSt_AFile)
		{
			fclose(pSt_AFile);
		}
		if (NULL != pSt_VFile)
		{
			fclose(pSt_VFile);
		}
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}