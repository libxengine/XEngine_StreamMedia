#include "pch.h"
#include "ModuleHelp_SrtCore.h"
/********************************************************************
//    Created:     2023/07/28  13:51:06
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Srt\ModuleHelp_SrtCore.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Srt
//    File Base:   ModuleHelp_SrtCore
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     
//    History:
*********************************************************************/
CModuleHelp_SrtCore::CModuleHelp_SrtCore()
{
	srt_startup();
	srt_setloglevel(srt_logging::LogLevel::fatal);
}
CModuleHelp_SrtCore::~CModuleHelp_SrtCore()
{
	srt_cleanup();
}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleHelp_SrtCore_Start
函数功能：启动SRT
 参数.一：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要绑定的端口
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_Start(int nPort)
{
	ModuleHelp_IsErrorOccur = false;

	hSRTSocket = srt_create_socket();
	if (hSRTSocket < 0)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_CRAETE;
		return false;
	}
	bool bFlag = false;
	srt_setsockopt(hSRTSocket, 0, SRTO_SNDSYN, (const void*)&bFlag, sizeof(bool));
	srt_setsockopt(hSRTSocket, 0, SRTO_RCVSYN, (const void*)&bFlag, sizeof(bool));

	sockaddr_in st_SockBind = { '\0' };
	st_SockBind.sin_family = AF_INET;
	st_SockBind.sin_port = htons(nPort);
	st_SockBind.sin_addr.s_addr = INADDR_ANY;

	if (SRT_ERROR == srt_bind(hSRTSocket, (sockaddr*)&st_SockBind, sizeof(st_SockBind)))
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_BIND;
		return false;
	}
	if (SRT_ERROR == srt_listen(hSRTSocket, 100))
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_LISTEN;
		return false;
	}
	//创建EPOLL
	hSRTEPoll = srt_epoll_create();
	if (hSRTEPoll < 0)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_EPOLL;
		return false;
	}
	int nSrtEvent = SRT_EPOLL_IN | SRT_EPOLL_ERR;
	if (SRT_ERROR == srt_epoll_add_usock(hSRTEPoll, hSRTSocket, &nSrtEvent))
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_ADD;
		return false;
	}
	bRun = true;
	pSDTThread = make_unique<thread>(ModuleHelp_SrtCore_Thread, this);
	if (NULL == pSDTThread)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_THREAD;
		return false;
	}
	return true;
}
/********************************************************************
函数名称：ModuleHelp_SrtCore_SetCallback
函数功能：设置数据回调函数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_SetCallback(CALLBACK_NETCORE_SOCKET_NETEVENT_LOGIN fpCallePoll_Login, CALLBACK_NETCORE_SOCKET_NETEVENT_RECV fpCallePoll_Recv, CALLBACK_NETCORE_SOCKET_NETEVENT_LEAVE fpCallePoll_Leave, XPVOID lPLogin /* = NULL */, XPVOID lPRecv /* = NULL */, XPVOID lPLeave /* = NULL */)
{
	ModuleHelp_IsErrorOccur = false;

	m_lLogin = lPLogin;
	m_lRecv = lPRecv;
	m_lLeave = lPLeave;
	lpCall_Login = fpCallePoll_Login;
	lpCall_Recv = fpCallePoll_Recv;
	lpCall_Leave = fpCallePoll_Leave;
	return true;
}
/********************************************************************
函数名称：ModuleHelp_SrtCore_Destory
函数功能：销毁SRT服务
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_Destory()
{
	ModuleHelp_IsErrorOccur = false;

	bRun = false;
	srt_close(hSRTSocket);

	if (NULL != pSDTThread)
	{
		pSDTThread->join();
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                             保护函数
//////////////////////////////////////////////////////////////////////////
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_Accept(SRTSOCKET hSRTSocket)
{
	ModuleHelp_IsErrorOccur = false;

	SRTCORE_CLIENTINFO st_SRTClient;
	sockaddr_storage st_ClientAddr;

	memset(&st_SRTClient, '\0', sizeof(SRTCORE_CLIENTINFO));
	int nADDRLen = sizeof(st_ClientAddr);

	st_SRTClient.hSocket = srt_accept(hSRTSocket, (sockaddr*)&st_ClientAddr, &nADDRLen);
	if (SRT_INVALID_SOCK == st_SRTClient.hSocket)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_ACCEPT;
		return false;
	}
	XCHAR tszAddrBuffer[128];
	XCHAR tszPortBuffer[128];

	memset(tszAddrBuffer, '\0', sizeof(tszAddrBuffer));
	memset(tszPortBuffer, '\0', sizeof(tszPortBuffer));
	
	getnameinfo((sockaddr*)&st_ClientAddr, nADDRLen, tszAddrBuffer, sizeof(tszAddrBuffer), tszPortBuffer, sizeof(tszPortBuffer), NI_NUMERICHOST | NI_NUMERICSERV);

	int nEPollEvent = SRT_EPOLL_IN | SRT_EPOLL_ERR;
	if (SRT_ERROR == srt_epoll_add_usock(hSRTEPoll, st_SRTClient.hSocket, &nEPollEvent))
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_ADD;
		return false;
	}
	_xstprintf(st_SRTClient.tszClientAddr, _X("%s:%s"), tszAddrBuffer, tszPortBuffer);

	st_Locker.lock();
	stl_MapClients.insert(make_pair(st_SRTClient.hSocket, st_SRTClient));
	st_Locker.unlock();
	lpCall_Login(st_SRTClient.tszClientAddr, st_SRTClient.hSocket, m_lLogin);
	return true;
}
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_Recv(SRTSOCKET hSocket)
{
	ModuleHelp_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapClients.find(hSocket);
	if (stl_MapIterator == stl_MapClients.end())
	{
		st_Locker.unlock_shared();
		return false;
	}
	SRTCORE_CLIENTINFO st_SRTClient = stl_MapIterator->second;
	st_Locker.unlock_shared();

	XCHAR tszMsgBuffer[1500];
	while (true)
	{
		int nRet = srt_recvmsg(hSocket, tszMsgBuffer, sizeof(tszMsgBuffer));
		if (SRT_ERROR == nRet)
		{
			if (SRT_EASYNCRCV != srt_getlasterror(NULL))
			{
				return false;
			}
			break;
		}
		lpCall_Recv(st_SRTClient.tszClientAddr, hSocket, tszMsgBuffer, nRet, m_lRecv);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                             线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CALLBACK CModuleHelp_SrtCore::ModuleHelp_SrtCore_Thread(XPVOID lParam)
{
	CModuleHelp_SrtCore* pClass_This = (CModuleHelp_SrtCore*)lParam;

	while (pClass_This->bRun)
	{
		int nSRTCount = 100;
		SRTSOCKET hSRTClient[100];

		int nSRTIndex = srt_epoll_wait(pClass_This->hSRTEPoll, &hSRTClient[0], &nSRTCount, 0, 0, 100, 0, 0, 0, 0);
		for (int i = 0; i < nSRTIndex; i++)
		{
			SRT_SOCKSTATUS nSRTStatus = srt_getsockstate(hSRTClient[i]);
			if ((nSRTStatus == SRTS_BROKEN) || (nSRTStatus == SRTS_NONEXIST) || (nSRTStatus == SRTS_CLOSED))
			{
				srt_close(hSRTClient[i]);
				continue;
			}
			else if (hSRTClient[i] == pClass_This->hSRTSocket)
			{
				pClass_This->ModuleHelp_SrtCore_Accept(pClass_This->hSRTSocket);
			}
			else
			{
				pClass_This->ModuleHelp_SrtCore_Recv(hSRTClient[i]);
			}
		}
	}
	return 0;
}