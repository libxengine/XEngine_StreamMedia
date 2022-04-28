#include "pch.h"
#include "ModuleSession_JT1078Server.h"
/********************************************************************
//    Created:     2022/04/28  14:21:14
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_JT1078\ModuleSession_JT1078Server.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_JT1078
//    File Base:   ModuleSession_JT1078Server
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JT1078服务器会话
//    History:
*********************************************************************/
CModuleSession_JT1078Server::CModuleSession_JT1078Server()
{
}
CModuleSession_JT1078Server::~CModuleSession_JT1078Server()
{
}
//////////////////////////////////////////////////////////////////////////
//                    公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_JT1078Server_Create
函数功能：创建一个服务器会话管理器
 参数.一：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.二：nChannel
  In/Out：In
  类型：nChannel
  可空：N
  意思：输入设备通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_JT1078Server::ModuleSession_JT1078Server_Create(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	Session_IsErrorOccur = FALSE;

	if (NULL == lpszDeviceNumber)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_PARAMENT;
		return FALSE;
	}
	//申请内存
	SESSION_RTPPACKET* pSt_RTPPacket = new SESSION_RTPPACKET;
	if (NULL == pSt_RTPPacket)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_MALLOC;
		return FALSE;
	}
	memset(pSt_RTPPacket, '\0', sizeof(SESSION_RTPPACKET));
	pSt_RTPPacket->pStl_ListBuffer = new list<SESSION_MSGBUFFER>;
	if (NULL == pSt_RTPPacket->pStl_ListBuffer)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_MALLOC;
		return FALSE;
	}
	//是否存在
	st_Locker.lock();
	unordered_map<string, unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>>>::iterator stl_MapIteratorDevice = stl_MapServer.find(lpszDeviceNumber);
	if (stl_MapIteratorDevice == stl_MapServer.end())
	{
		//不存在就创建
		unordered_map<BOOL, SESSION_RTPPACKET*> stl_MapLive;
		unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>> stl_MapChannel;

		stl_MapLive.insert(make_pair(bLive, pSt_RTPPacket));
		stl_MapChannel.insert(make_pair(nChannel, stl_MapLive));
		stl_MapServer.insert(make_pair(lpszDeviceNumber, stl_MapChannel));
	}
	else
	{
		//存在,需要判断
		unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>>::iterator stl_MapIteratorChannel = stl_MapIteratorDevice->second.find(nChannel);
		if (stl_MapIteratorChannel == stl_MapIteratorDevice->second.end())
		{
			unordered_map<BOOL, SESSION_RTPPACKET*> stl_MapLive;
			stl_MapLive.insert(make_pair(bLive, pSt_RTPPacket));

			stl_MapIteratorDevice->second.insert(make_pair(nChannel, stl_MapLive));
		}
		else
		{
			unordered_map<BOOL, SESSION_RTPPACKET*>::iterator stl_MapIteratorLive = stl_MapIteratorChannel->second.find(bLive);
			if (stl_MapIteratorLive == stl_MapIteratorChannel->second.end())
			{
				stl_MapIteratorChannel->second.insert(make_pair(bLive, pSt_RTPPacket));
			}
			else
			{
				Session_IsErrorOccur = TRUE;
				Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_EXIST;
				st_Locker.unlock();
				return FALSE;
			}
		}
	}
	st_Locker.unlock();
	return TRUE;
}
/********************************************************************
函数名称：ModuleSession_JT1078Server_Destroy
函数功能：销毁一个管理器
 参数.一：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.二：nChannel
  In/Out：In
  类型：nChannel
  可空：N
  意思：输入设备通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_JT1078Server::ModuleSession_JT1078Server_Destroy(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	Session_IsErrorOccur = FALSE;

	if (NULL == lpszDeviceNumber)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_PARAMENT;
		return FALSE;
	}
	//设备编号是否存在
	st_Locker.lock();
	unordered_map<string, unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>>>::iterator stl_MapIteratorDevice = stl_MapServer.find(lpszDeviceNumber);
	if (stl_MapIteratorDevice == stl_MapServer.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_NOTDEVICE;
		st_Locker.unlock();
		return FALSE;
	}
	//通道是否存在
	unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>>::iterator stl_MapIteratorChannel = stl_MapIteratorDevice->second.find(nChannel);
	if (stl_MapIteratorChannel == stl_MapIteratorDevice->second.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_NOTCHANNEL;
		st_Locker.unlock();
		return FALSE;
	}
	//直播还是录像
	unordered_map<BOOL, SESSION_RTPPACKET*>::iterator stl_MapIteratorLive = stl_MapIteratorChannel->second.find(bLive);
	if (stl_MapIteratorLive == stl_MapIteratorChannel->second.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_NOTLIVE;
		st_Locker.unlock();
		return FALSE;
	}
	stl_MapIteratorLive->second->st_Locker.lock();
	list<SESSION_MSGBUFFER>::iterator stl_ListIterator = stl_MapIteratorLive->second->pStl_ListBuffer->begin();
	for (; stl_ListIterator != stl_MapIteratorLive->second->pStl_ListBuffer->end(); stl_ListIterator++)
	{
		free(stl_ListIterator->ptszMsgBuffer);
		stl_ListIterator->ptszMsgBuffer = NULL;
	}
	stl_MapIteratorLive->second->pStl_ListBuffer->clear();
	delete stl_MapIteratorLive->second->pStl_ListBuffer;
	stl_MapIteratorLive->second->pStl_ListBuffer = NULL;
	stl_MapIteratorLive->second->st_Locker.unlock();

	delete stl_MapIteratorLive->second;
	stl_MapIteratorLive->second = NULL;
	stl_MapIteratorChannel->second.erase(stl_MapIteratorLive);

	if (stl_MapIteratorChannel->second.empty())
	{
		stl_MapIteratorDevice->second.erase(stl_MapIteratorChannel);
	}
	if (stl_MapIteratorDevice->second.empty())
	{
		stl_MapServer.erase(stl_MapIteratorDevice);
	}
	st_Locker.unlock();
	return TRUE;
}
/********************************************************************
函数名称：ModuleSession_JT1078Server_Insert
函数功能：数据包插入
 参数.一：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.二：nChannel
  In/Out：In
  类型：nChannel
  可空：N
  意思：输入设备通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入直播还是录像
 参数.四：pSt_RTPHdr
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入直播还是录像
 参数.五：pSt_RTPTail
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入直播还是录像
 参数.六：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要插入的数据
 参数.七：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入数据大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_JT1078Server::ModuleSession_JT1078Server_Insert(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, XENGINE_RTPPACKETHDR2016* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == lpszDeviceNumber) || (NULL == lpszMsgBuffer))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_PARAMENT;
		return FALSE;
	}
	st_Locker.lock_shared();
	unordered_map<string, unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>>>::const_iterator stl_MapDeviceIterator = stl_MapServer.find(lpszDeviceNumber);
	if (stl_MapDeviceIterator == stl_MapServer.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_NOTDEVICE;
		st_Locker.unlock_shared();
		return FALSE;
	}
	unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>>::const_iterator stl_MapChannelIterator = stl_MapDeviceIterator->second.find(nChannel);
	if (stl_MapChannelIterator == stl_MapDeviceIterator->second.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_NOTCHANNEL;
		st_Locker.unlock_shared();
		return FALSE;
	}
	unordered_map<BOOL, SESSION_RTPPACKET*>::const_iterator stl_MapLiveIterator = stl_MapChannelIterator->second.find(bLive);
	if (stl_MapLiveIterator == stl_MapChannelIterator->second.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_NOTLIVE;
		st_Locker.unlock_shared();
		return FALSE;
	}
	SESSION_MSGBUFFER st_MsgBuffer;
	memset(&st_MsgBuffer, '\0', sizeof(SESSION_MSGBUFFER));

	st_MsgBuffer.ptszMsgBuffer = (TCHAR*)malloc(nMsgLen + 1);
	if (NULL == st_MsgBuffer.ptszMsgBuffer)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_MALLOC;
		st_Locker.unlock_shared();
		return FALSE;
	}
	memset(st_MsgBuffer.ptszMsgBuffer, '\0', nMsgLen + 1);

	st_MsgBuffer.nMsgLen = nMsgLen;
	memcpy(st_MsgBuffer.ptszMsgBuffer, lpszMsgBuffer, nMsgLen);

	stl_MapLiveIterator->second->st_Locker.lock();;
	stl_MapLiveIterator->second->pStl_ListBuffer->push_back(st_MsgBuffer);
	stl_MapLiveIterator->second->st_Locker.unlock();
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：ModuleSession_JT1078Server_Get
函数功能：获取数据
 参数.一：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.二：nChannel
  In/Out：In
  类型：nChannel
  可空：N
  意思：输入设备通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入直播还是录像
 参数.四：ptszMsgBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出获取到的数据
 参数.五：pInt_MsgLen
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出数据大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_JT1078Server::ModuleSession_JT1078Server_Get(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == lpszDeviceNumber) || (NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_PARAMENT;
		return FALSE;
	}
	st_Locker.lock_shared();
	unordered_map<string, unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>>>::iterator stl_MapDeviceIterator = stl_MapServer.find(lpszDeviceNumber);
	if (stl_MapDeviceIterator == stl_MapServer.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_NOTDEVICE;
		*pInt_MsgLen = -1;
		st_Locker.unlock_shared();
		return FALSE;
	}
	unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>>::iterator stl_MapChannelIterator = stl_MapDeviceIterator->second.find(nChannel);
	if (stl_MapChannelIterator == stl_MapDeviceIterator->second.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_NOTCHANNEL;
		*pInt_MsgLen = -1;
		st_Locker.unlock_shared();
		return FALSE;
	}
	unordered_map<BOOL, SESSION_RTPPACKET*>::iterator stl_MapLiveIterator = stl_MapChannelIterator->second.find(bLive);
	if (stl_MapLiveIterator == stl_MapChannelIterator->second.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_NOTLIVE;
		*pInt_MsgLen = -1;
		st_Locker.unlock_shared();
		return FALSE;
	}
	stl_MapLiveIterator->second->st_Locker.lock();
	if (stl_MapLiveIterator->second->pStl_ListBuffer->empty())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_EMPTY;
		stl_MapLiveIterator->second->st_Locker.unlock();
		st_Locker.unlock_shared();
		return FALSE;
	}
	*pInt_MsgLen = stl_MapLiveIterator->second->pStl_ListBuffer->front().nMsgLen;
	memcpy(ptszMsgBuffer, stl_MapLiveIterator->second->pStl_ListBuffer->front().ptszMsgBuffer, stl_MapLiveIterator->second->pStl_ListBuffer->front().nMsgLen);

	free(stl_MapLiveIterator->second->pStl_ListBuffer->front().ptszMsgBuffer);
	stl_MapLiveIterator->second->pStl_ListBuffer->front().ptszMsgBuffer = NULL;
	stl_MapLiveIterator->second->pStl_ListBuffer->pop_front();

	stl_MapLiveIterator->second->st_Locker.unlock();
	st_Locker.unlock_shared();
	return TRUE;
}