#include "pch.h"
#include "ModuleSession_Server.h"
/********************************************************************
//    Created:     2022/07/04  14:16:09
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Server\ModuleSession_Server.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Server
//    File Base:   ModuleSession_Server
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     服务器会话
//    History:
*********************************************************************/
CModuleSession_Server::CModuleSession_Server()
{
}
CModuleSession_Server::~CModuleSession_Server()
{
}
//////////////////////////////////////////////////////////////////////////
//                    公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_Server_Create
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
bool CModuleSession_Server::ModuleSession_Server_Create(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszDeviceNumber)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	//申请内存
	SESSION_RTPPACKET* pSt_RTPPacket = new SESSION_RTPPACKET;
	if (NULL == pSt_RTPPacket)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_MALLOC;
		return false;
	}
	//是否存在
	st_Locker.lock();
	unordered_map<string, unordered_map<int, unordered_map<bool, SESSION_RTPPACKET*>>>::iterator stl_MapIteratorDevice = stl_MapServer.find(lpszDeviceNumber);
	if (stl_MapIteratorDevice == stl_MapServer.end())
	{
		//不存在就创建
		unordered_map<bool, SESSION_RTPPACKET*> stl_MapLive;
		unordered_map<int, unordered_map<bool, SESSION_RTPPACKET*>> stl_MapChannel;

		stl_MapLive.insert(make_pair(bLive, pSt_RTPPacket));
		stl_MapChannel.insert(make_pair(nChannel, stl_MapLive));
		stl_MapServer.insert(make_pair(lpszDeviceNumber, stl_MapChannel));
	}
	else
	{
		//存在,需要判断
		unordered_map<int, unordered_map<bool, SESSION_RTPPACKET*>>::iterator stl_MapIteratorChannel = stl_MapIteratorDevice->second.find(nChannel);
		if (stl_MapIteratorChannel == stl_MapIteratorDevice->second.end())
		{
			unordered_map<bool, SESSION_RTPPACKET*> stl_MapLive;
			stl_MapLive.insert(make_pair(bLive, pSt_RTPPacket));

			stl_MapIteratorDevice->second.insert(make_pair(nChannel, stl_MapLive));
		}
		else
		{
			unordered_map<bool, SESSION_RTPPACKET*>::iterator stl_MapIteratorLive = stl_MapIteratorChannel->second.find(bLive);
			if (stl_MapIteratorLive == stl_MapIteratorChannel->second.end())
			{
				stl_MapIteratorChannel->second.insert(make_pair(bLive, pSt_RTPPacket));
			}
			else
			{
				Session_IsErrorOccur = true;
				Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_EXIST;
				st_Locker.unlock();
				return false;
			}
		}
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Server_Destroy
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
bool CModuleSession_Server::ModuleSession_Server_Destroy(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszDeviceNumber)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	//设备编号是否存在
	st_Locker.lock();
	unordered_map<string, unordered_map<int, unordered_map<bool, SESSION_RTPPACKET*>>>::iterator stl_MapIteratorDevice = stl_MapServer.find(lpszDeviceNumber);
	if (stl_MapIteratorDevice == stl_MapServer.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTDEVICE;
		st_Locker.unlock();
		return false;
	}
	//通道是否存在
	unordered_map<int, unordered_map<bool, SESSION_RTPPACKET*>>::iterator stl_MapIteratorChannel = stl_MapIteratorDevice->second.find(nChannel);
	if (stl_MapIteratorChannel == stl_MapIteratorDevice->second.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTCHANNEL;
		st_Locker.unlock();
		return false;
	}
	//直播还是录像
	unordered_map<bool, SESSION_RTPPACKET*>::iterator stl_MapIteratorLive = stl_MapIteratorChannel->second.find(bLive);
	if (stl_MapIteratorLive == stl_MapIteratorChannel->second.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTLIVE;
		st_Locker.unlock();
		return false;
	}

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
	return true;
}
/********************************************************************
函数名称：ModuleSession_Server_SetPush
函数功能：设置推送句柄
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
 参数.四：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要设置的推送句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_Server::ModuleSession_Server_SetPush(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive, XHANDLE xhToken)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszDeviceNumber)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	st_Locker.lock_shared();
	unordered_map<string, unordered_map<int, unordered_map<bool, SESSION_RTPPACKET*>>>::iterator stl_MapDeviceIterator = stl_MapServer.find(lpszDeviceNumber);
	if (stl_MapDeviceIterator == stl_MapServer.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTDEVICE;
		st_Locker.unlock_shared();
		return false;
	}
	unordered_map<int, unordered_map<bool, SESSION_RTPPACKET*>>::iterator stl_MapChannelIterator = stl_MapDeviceIterator->second.find(nChannel);
	if (stl_MapChannelIterator == stl_MapDeviceIterator->second.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTCHANNEL;
		st_Locker.unlock_shared();
		return false;
	}
	unordered_map<bool, SESSION_RTPPACKET*>::iterator stl_MapLiveIterator = stl_MapChannelIterator->second.find(bLive);
	if (stl_MapLiveIterator == stl_MapChannelIterator->second.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTLIVE;
		st_Locker.unlock_shared();
		return false;
	}
	stl_MapLiveIterator->second->xhToken = xhToken;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_Server_GetPush
函数功能：获取推送句柄
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
 参数.四：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出获取到的推送句柄
返回值
  类型：句柄
  意思：返回获取的信息
备注：
*********************************************************************/
XHANDLE CModuleSession_Server::ModuleSession_Server_GetPush(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszDeviceNumber)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return NULL;
	}
	st_Locker.lock_shared();
	unordered_map<string, unordered_map<int, unordered_map<bool, SESSION_RTPPACKET*>>>::iterator stl_MapDeviceIterator = stl_MapServer.find(lpszDeviceNumber);
	if (stl_MapDeviceIterator == stl_MapServer.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTDEVICE;
		st_Locker.unlock_shared();
		return NULL;
	}
	unordered_map<int, unordered_map<bool, SESSION_RTPPACKET*>>::iterator stl_MapChannelIterator = stl_MapDeviceIterator->second.find(nChannel);
	if (stl_MapChannelIterator == stl_MapDeviceIterator->second.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTCHANNEL;
		st_Locker.unlock_shared();
		return NULL;
	}
	unordered_map<bool, SESSION_RTPPACKET*>::iterator stl_MapLiveIterator = stl_MapChannelIterator->second.find(bLive);
	if (stl_MapLiveIterator == stl_MapChannelIterator->second.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTLIVE;
		st_Locker.unlock_shared();
		return NULL;
	}
	XHANDLE xhToken = stl_MapLiveIterator->second->xhToken;
	st_Locker.unlock_shared();
	return xhToken;
}