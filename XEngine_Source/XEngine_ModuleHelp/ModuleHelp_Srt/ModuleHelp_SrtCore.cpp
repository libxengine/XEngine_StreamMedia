﻿#include "pch.h"
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
}
CModuleHelp_SrtCore::~CModuleHelp_SrtCore()
{
}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleHelp_SrtCore_Init
函数功能：初始化SRT服务
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_Init()
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT
	srt_startup();
	srt_setloglevel(srt_logging::LogLevel::fatal);
#endif

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

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT
	srt_cleanup();
#endif
	return true;
}
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

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT
	hSRTSocket = srt_create_socket();
	if (hSRTSocket < 0)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_CRAETE;
		return false;
	}
	bool bFlag = false;
	SRT_TRANSTYPE nType = SRTT_LIVE;
	srt_setsockopt(hSRTSocket, 0, SRTO_TRANSTYPE, &nType, sizeof(nType));
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
#endif
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
函数名称：ModuleHelp_SrtCore_Send
函数功能：发送指定数据给客户端
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要操作的客户端
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：发送的数据
 参数.三：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：发送的大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT
	if (NULL == lpszClientAddr)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_PARAMENT;
		return false;
	}
	bool bFound = false;
	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapClients.begin();
	for (; stl_MapIterator != stl_MapClients.end(); stl_MapIterator++)
	{
		if (0 == _tcsxnicmp(lpszClientAddr, stl_MapIterator->second.tszClientAddr, _tcsxlen(lpszClientAddr)))
		{
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	//int nSRTEvent = SRT_EPOLL_OUT | SRT_EPOLL_ERR;
	//srt_epoll_add_usock(hSRTEPoll, hSRTSocket, &nSRTEvent);

	int nRet = 0;
	int nSendCount = 0;
	int nLeftCount = nMsgLen;
	XSOCKET hSocket = stl_MapIterator->second.hSocket;
	
	while (true)
	{
		if (nLeftCount > 1300)
		{
			nRet = srt_send(hSocket, lpszMsgBuffer + nSendCount, 1300);
			nSendCount += 1300;
			nLeftCount -= 1300;
		}
		else
		{
			nRet = srt_send(hSocket, lpszMsgBuffer + nSendCount, nLeftCount);
			nSendCount += nLeftCount;
			nLeftCount -= nLeftCount;
		}

		if (nSendCount == nMsgLen)
		{
			break;
		}
		
		if (-1 == nRet)
		{
			ModuleHelp_IsErrorOccur = true;
			ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_SENDFAILED;
			st_Locker.unlock_shared();
			return false;
		}
	}
	st_Locker.unlock_shared();
	//nSRTEvent = SRT_EPOLL_IN | SRT_EPOLL_ERR;
	//srt_epoll_update_usock(hSRTEPoll, hSRTSocket, &nSRTEvent);
#endif
	return true;
}
/********************************************************************
函数名称：ModuleHelp_SrtCore_GetStreamID
函数功能：获取流信息
 参数.一：hSocket
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的客户端
 参数.二：ptszSMSAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取到的推流地址
 参数.三：pbPublish
  In/Out：Out
  类型：逻辑型指针
  可空：N
  意思：输出这个地址是推流还是拉流
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_GetStreamID(XSOCKET hSocket, XCHAR* ptszSMSAddr, bool* pbPublish)
{
	ModuleHelp_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapClients.find(hSocket);
	if (stl_MapIterator == stl_MapClients.end())
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	if (0 == stl_MapIterator->second.nIDLen)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_GETID;
		st_Locker.unlock_shared();
		return false;
	}
	//#!::h=live/livestream,m=publish
	XCHAR tszTmpStr[MAX_PATH];
	XCHAR tszSMSAddr[MAX_PATH];
	XCHAR tszSMSMode[MAX_PATH];

	memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
	memset(tszSMSMode, '\0', sizeof(tszSMSMode));

	BaseLib_String_GetKeyValue(stl_MapIterator->second.tszStreamBuffer + 4, _X(","), tszSMSAddr, tszSMSMode);
	BaseLib_String_GetKeyValue(tszSMSAddr, _X("="), tszTmpStr, ptszSMSAddr);
	
	if (NULL != _tcsxstr(tszSMSMode, _X("publish")))
	{
		*pbPublish = true;
	}
	else 
	{
		*pbPublish = false;
	}
	st_Locker.unlock_shared();

	return true;
}
/********************************************************************
函数名称：ModuleHelp_SrtCore_Close
函数功能：关闭一个指定的客户端
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入关闭的客户端低脂
 参数.二：hSocket
  In/Out：In
  类型：句柄
  可空：Y
  意思：或者输入客户端句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_Close(LPCXSTR lpszClientAddr /* = NULL */, XSOCKET hSocket /* = 0 */)
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT
	if (0 != hSocket)
	{
		st_Locker.lock();
		auto stl_MapIterator = stl_MapClients.find(hSocket);
		if (stl_MapIterator != stl_MapClients.end())
		{
			srt_epoll_remove_usock(hSRTEPoll, stl_MapIterator->second.hSocket);
			srt_close(stl_MapIterator->second.hSocket);

			stl_MapClients.erase(stl_MapIterator);
		}
		st_Locker.unlock();
	}
#endif
	return true;
}
/********************************************************************
函数名称：ModuleHelp_SrtCore_Stop
函数功能：销毁SRT服务
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_Stop()
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT
	bRun = false;
	srt_close(hSRTSocket);
	srt_epoll_release(hSRTEPoll);

	if (NULL != pSDTThread)
	{
		pSDTThread->join();
	}
#endif
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                             保护函数
//////////////////////////////////////////////////////////////////////////
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_Accept(XSOCKET hSRTSocket)
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT
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
	
	st_SRTClient.nIDLen = 512;
	int nRet = srt_getsockflag(st_SRTClient.hSocket, SRTO_STREAMID, st_SRTClient.tszStreamBuffer, &st_SRTClient.nIDLen);
	if (SRT_ERROR == nRet)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_SRT_GETID;
		return false;
	}
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
#endif
	return true;
}
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_Recv(XSOCKET hSocket)
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT
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
#endif
	return true;
}
bool CModuleHelp_SrtCore::ModuleHelp_SrtCore_Leave(XSOCKET hSocket)
{
	ModuleHelp_IsErrorOccur = false;

#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT
	if (0 != hSocket)
	{
		XCHAR tszClientAddr[128];
		memset(tszClientAddr, '\0', sizeof(tszClientAddr));

		st_Locker.lock();
		auto stl_MapIterator = stl_MapClients.find(hSocket);
		if (stl_MapIterator != stl_MapClients.end())
		{
			_tcsxcpy(tszClientAddr, stl_MapIterator->second.tszClientAddr);
			srt_epoll_remove_usock(hSRTEPoll, stl_MapIterator->second.hSocket);
			srt_close(stl_MapIterator->second.hSocket);

			stl_MapClients.erase(stl_MapIterator);
		}
		st_Locker.unlock();

		lpCall_Leave(tszClientAddr, hSocket, m_lLeave);
	}
#endif
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                             线程函数
//////////////////////////////////////////////////////////////////////////
XHTHREAD CALLBACK CModuleHelp_SrtCore::ModuleHelp_SrtCore_Thread(XPVOID lParam)
{
#if 1 == _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT
	CModuleHelp_SrtCore* pClass_This = (CModuleHelp_SrtCore*)lParam;

	while (pClass_This->bRun)
	{
		int nSRTCount = 100;
		XSOCKET hSRTClient[100];

		int nSRTIndex = srt_epoll_wait(pClass_This->hSRTEPoll, (SRTSOCKET *)hSRTClient, &nSRTCount, NULL, NULL, -1, NULL, NULL, NULL, NULL);
		for (int i = 0; i < nSRTIndex; i++)
		{
			SRT_SOCKSTATUS nSRTStatus = srt_getsockstate(hSRTClient[i]);
			if ((nSRTStatus == SRTS_BROKEN) || (nSRTStatus == SRTS_NONEXIST) || (nSRTStatus == SRTS_CLOSED))
			{
				pClass_This->ModuleHelp_SrtCore_Leave(hSRTClient[i]);
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
#endif
	return 0;
}